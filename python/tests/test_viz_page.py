"""The page of a diagram in a browser, with the input of a user (Playwright:
pip install playwright; playwright install chromium). The page talks to an
editor as in VS Code: what it posts is kept, and the editor's messages are
posted to it.

CHASE_TEST_ELK=PATH puts a local elk.bundled.js in the pages (else ELK
comes from a CDN)."""

import glob
import os
import re

import pytest

from chase_cps import viz

pw = pytest.importorskip("playwright.sync_api")
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DEMOS = os.path.join(ROOT, "demos")
pytestmark = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed")
SPECS = sorted(glob.glob(os.path.join(DEMOS, "**", "*.ltl"), recursive=True))
SVA = [f for f in sorted(glob.glob(os.path.join(DEMOS, "**", "sva", "*.sv"), recursive=True))]
# C with the clauses of ESBMC, CBMC, ACSL; SMV with its specifications (no check is run)
CSPECS = sorted(glob.glob(os.path.join(os.path.dirname(__file__), "data", "*_clauses.*")))

EDITOR = """
window.posted = [];
window.acquireVsCodeApi = () => ({ postMessage: m => window.posted.push(m) });
"""


@pytest.fixture(scope="module")
def browser():
    with pw.sync_playwright() as p:
        try:
            b = p.chromium.launch()
        except Exception as e:          # no browser installed
            pytest.skip(f"no Chromium for Playwright: {e}")
        yield b
        b.close()


def open_page(browser, tmp_path, d, editor=True):
    page = browser.new_page(viewport={"width": 1200, "height": 800})
    errors = []
    page.on("pageerror", lambda e: errors.append(str(e)))
    if editor:
        page.add_init_script(EDITOR)
    f = tmp_path / "page.html"
    f.write_text(viz.html(d, elk=os.environ.get("CHASE_TEST_ELK")))
    page.goto(f.as_uri())
    page.wait_for_selector("[data-box]", timeout=30000)
    page.wait_for_timeout(100)
    return page, errors


def posted(page):
    out = page.evaluate("window.posted.splice(0)")
    return [m for m in out if m.get("type") == "chase-reveal"]


def rows(page):
    """Where the top boxes are: wired to each other, the column of each and
    its place in it; else the rows (by their top), each from left to right."""
    if page.evaluate("(DATA.edges || []).length > 0"):
        return page.evaluate("""() => { const bs = DATA.nodes.map(n => {
            const r = document.querySelector(`[data-box="${n.id}"] > rect`).getBoundingClientRect(); return [n.id, r.x, r.y]; });
          const xs = []; bs.forEach(b => { if (!xs.some(x => Math.abs(x - b[1]) < 8)) xs.push(b[1]); }); xs.sort((a, b) => a - b);
          const col = b => xs.findIndex(x => Math.abs(x - b[1]) < 8);
          return bs.map(b => [b[0], col(b), bs.filter(d => col(d) === col(b)).sort((p, q) => p[2] - q[2]).indexOf(b)]); }""")
    tops = page.evaluate("""() => DATA.nodes.map(n => {
        const r = document.querySelector(`[data-box="${n.id}"] > rect`);
        return r ? [n.id, r.getBoundingClientRect().y, r.getBoundingClientRect().x] : null; }).filter(Boolean)""")
    out = {}
    for i, y, x in tops:
        out.setdefault(round(y / 4), []).append((x, i))
    return [[i for _, i in sorted(r)] for _, r in sorted(out.items())]


def boxes(page):
    return page.evaluate("""() => [...document.querySelectorAll('[data-box]')].map(e => {
        const n = byId[e.getAttribute('data-box')], r = e.querySelector('rect').getBoundingClientRect();
        return {id: n.id, title: n.title, source: n.source || null, contract: !!n.contract,
                x: r.x, y: r.y, w: r.width, h: r.height}; })""")


def term_rows(page, box_id):
    return page.evaluate("""id => [...document.querySelectorAll(`[data-box="${id}"] .trow[data-line]`)]
        .filter(r => r.closest('[data-box]').getAttribute('data-box') === id).map(r => {
          const b = r.getBoundingClientRect();
          return {line: +r.getAttribute('data-line'), last: +r.getAttribute('data-last'),
                  x: b.x + 30, y: b.y + b.height / 2}; })""", box_id)


def name_of(page, b):
    """The middle of the name of box b, where a click opens it."""
    return page.evaluate("""id => { const t = document.querySelector(`[data-box="${id}"] > text`);
        const r = t.getBoundingClientRect(); return [r.x + r.width / 2, r.y + r.height / 2]; }""", b["id"])


def bring(page, b):
    """b in the window: the view moved so that its head is at the middle."""
    page.evaluate("""([id]) => {
        const r = document.querySelector(`[data-box="${id}"] > rect`).getBoundingClientRect(),
              v = document.getElementById('view').getBoundingClientRect();
        cam.x += v.width / 2 - r.x - 40; cam.y += v.height / 2 - r.y; place(); }""", [b["id"]])
    return next(x for x in boxes(page) if x["id"] == b["id"])


@pytest.mark.parametrize("path", SPECS + SVA + CSPECS, ids=lambda f: os.path.relpath(f, DEMOS))
def test_each_box_and_term_goes_to_its_line(browser, tmp_path, path, monkeypatch):
    monkeypatch.chdir(os.path.dirname(path))
    page, errors = open_page(browser, tmp_path, viz.from_spec(path))
    first = rows(page)
    for b in [b for b in boxes(page) if b["source"] and b["contract"]]:
        b = bring(page, b)
        page.mouse.dblclick(b["x"] + min(b["w"], 300) - 12, b["y"] + min(b["h"] - 6, 40))   # the body
        assert posted(page) == [{"type": "chase-reveal", "line": b["source"]["line"]}], b["title"]
        page.mouse.click(*name_of(page, b))                             # the name: open
        page.wait_for_timeout(50)
        assert rows(page) == first, f"opening {b['title']} moved the boxes to other rows"
        ts = term_rows(page, b["id"])
        want = [ab for p in "AG" for ab in b["source"].get(p, [])]
        shown = {(t["line"], t["last"]) for t in ts}
        assert shown <= {tuple(x) for x in want}, b["title"]
        if shown != {tuple(x) for x in want}:                                 # a long contract: the rest in the panel
            more = page.evaluate("""id => { const t = [...document.querySelectorAll(`[data-box="${id}"] text`)]
                .find(t => /more lines/.test(t.textContent) && t.closest('[data-box]').getAttribute('data-box') === id);
                const r = t.getBoundingClientRect(); return [r.x + 10, r.y + r.height / 2]; }""", b["id"])
            page.mouse.click(*more)
            trs = page.evaluate("""() => [...document.querySelectorAll('#popview tr[data-line]')].map(r => {
                const b = r.getBoundingClientRect(); return [+r.getAttribute('data-line'), b.x + 40, b.y + b.height / 2]; })""")
            assert sorted({t[0] for t in trs}) == sorted({x[0] for x in want}), b["title"]
            line, x, y = next(t for t in trs if 0 < t[2] < 800)
            page.mouse.dblclick(x, y)
            assert posted(page) == [{"type": "chase-reveal", "line": line}]
            page.evaluate("closePop()")
        for t in ts:
            if t["y"] < 0 or t["y"] > 800:
                continue
            page.mouse.dblclick(t["x"], t["y"])
            assert posted(page) == [{"type": "chase-reveal", "line": t["line"]}], (b["title"], t)
        for a, z in want:                                                      # the cursor at a term
            page.evaluate("m => window.postMessage(m, '*')",
                          {"type": "chase-select", "line": b["source"]["line"], "at": z})
            page.wait_for_timeout(20)
            sel = page.evaluate("""() => [[...document.querySelectorAll('[data-box].sel')].map(e => e.getAttribute('data-box')),
                [...document.querySelectorAll('.trow.sel')].map(r => r.getAttribute('data-hidden') ? JSON.parse(r.getAttribute('data-hidden'))
                  : [[+r.getAttribute('data-line'), +r.getAttribute('data-last')]])]""")
            assert b["id"] in sel[0]                     # the term, or the line of the hidden terms, is marked
            assert sel[1] and all(any(x <= z <= y for x, y in rs) for rs in sel[1]), (b["title"], z, sel[1])
        b = next(x for x in boxes(page) if x["id"] == b["id"])
        page.mouse.click(*name_of(page, b))                             # close
        page.wait_for_timeout(50)
    assert rows(page) == first
    assert not errors, errors


@pytest.mark.skipif(not SPECS, reason="the demos are not checked out")
def test_the_view_moves_and_exports(browser, tmp_path, monkeypatch):
    path = next(f for f in SPECS if f.endswith("charger.ltl"))
    monkeypatch.chdir(os.path.dirname(path))
    page, errors = open_page(browser, tmp_path, viz.from_spec(path))
    view = lambda: page.evaluate("() => [cam.x, cam.y, cam.k]")
    x0, y0, k0 = view()
    page.mouse.move(600, 760)
    page.mouse.down()
    page.mouse.move(650, 780, steps=5)
    page.mouse.up()
    x1, y1, k1 = view()
    assert (round(x1 - x0), round(y1 - y0), k1) == (50, 20, k0)
    page.mouse.wheel(0, -300)
    assert view()[2] > k0
    page.keyboard.press("0")                                                   # fit
    page.wait_for_timeout(50)
    box = page.evaluate("() => { const r = camera.getBoundingClientRect(); return [r.x, r.y, r.right, r.bottom]; }")
    assert box[0] >= 0 and box[1] >= 0 and box[2] <= 1200 and box[3] <= 800
    page.evaluate("m => window.postMessage(m, '*')", {"type": "chase-select", "line": 31, "at": 31})
    svg = page.evaluate("() => exported().text")
    ok = page.evaluate("""s => { const d = new DOMParser().parseFromString(s, 'image/svg+xml');
        return !d.querySelector('parsererror') && !d.querySelector('.sel') && !!d.querySelector('[data-box]'); }""", svg)
    assert ok
    assert not errors, errors


@pytest.mark.skipif(not SPECS, reason="the demos are not checked out")
def test_a_new_drawing_keeps_the_view(browser, tmp_path, monkeypatch):
    path = next(f for f in SPECS if f.endswith("charger.ltl"))
    monkeypatch.chdir(os.path.dirname(path))
    d = viz.from_spec(path)
    page, errors = open_page(browser, tmp_path, d)
    b = next(b for b in boxes(page) if b["title"] == "Greedy")
    page.mouse.click(*name_of(page, b))
    first, view = rows(page), page.evaluate("() => [cam.x, cam.y, cam.k]")
    page.evaluate("d => window.postMessage({type: 'chase-data', data: d}, '*')", d)    # the same file again
    page.wait_for_timeout(300)
    assert page.evaluate("() => [...open]") and rows(page) == first
    assert page.evaluate("() => [cam.x, cam.y, cam.k]") == view
    page.evaluate("m => window.postMessage(m, '*')", {"type": "chase-stale", "reason": "line 3:4: mismatched input"})
    assert page.is_visible("#stale")
    assert not errors, errors


def test_a_core_goes_to_its_lines(browser, tmp_path, monkeypatch):
    from chase_cps.backends import ADAPTERS
    if not ADAPTERS["slugs"].available() or not SPECS:
        pytest.skip("slugs, or the demos, not found")
    path = next(f for f in SPECS if f.endswith("charger.ltl"))
    monkeypatch.chdir(os.path.dirname(path))
    d = viz.from_spec(path, ["Greedy"], realizable=True, open=["Greedy"])
    page, errors = open_page(browser, tmp_path, d)
    g = next(n for n in d["nodes"] if n["title"] == "Greedy")
    core = next(c["core"] for c in g["checks"] if c.get("core"))
    want = sorted(g["source"]["G"][i][0] for i in core["G"])
    red = page.evaluate("""() => [...document.querySelectorAll('.trow')].filter(r => r.querySelector("rect[fill='#f6dcd9']"))
        .map(r => { const b = r.getBoundingClientRect(); return [+r.getAttribute('data-line'), b.x + 30, b.y + b.height / 2]; })""")
    assert sorted(l for l, _, _ in red) == want
    for line, x, y in red:
        page.mouse.dblclick(x, y)
        assert posted(page) == [{"type": "chase-reveal", "line": line}]
    assert not errors, errors


@pytest.mark.skipif(not SPECS, reason="the demos are not checked out")
def test_the_check_button_and_the_checks_running(browser, tmp_path, monkeypatch):
    path = next(f for f in SPECS if f.endswith("charger.ltl"))
    monkeypatch.chdir(os.path.dirname(path))
    page, errors = open_page(browser, tmp_path, viz.from_spec(path))
    run = page.locator("#run")
    assert run.is_visible()
    run.click()
    assert page.evaluate("window.posted.splice(0)") == [{"type": "chase-check"}]
    page.evaluate("m => window.postMessage(m, '*')", {"type": "chase-busy", "on": True})
    page.wait_for_timeout(300)
    # three dots where each result will be: A ∧ G of each contract, the ⪯ of the system
    boxes_with_contract = page.evaluate("DATA.nodes.length + DATA.nodes.reduce((k, n) => k + (n.children || []).length, 0)")
    dots = page.evaluate("document.querySelectorAll('.pending').length")
    assert dots == boxes_with_contract + 1
    widths = page.evaluate("""() => [...document.querySelectorAll('.pending rect')].map(r => +r.getAttribute('width'))""")
    assert set(widths) <= {page.evaluate("tagWidth('A∧G')"), page.evaluate("tagWidth('⪯')")}
    # results known one by one: in place of their dots at once
    for e in ({"check": "feasible", "result": "unsat", "of": "Greedy"},
              {"check": "refines", "result": "valid", "refined": "Robot1+Robot2+Greedy", "abstract": "System"}):
        page.evaluate("m => window.postMessage(m, '*')", {"type": "chase-progress", "event": e})
    page.wait_for_timeout(400)
    assert page.evaluate("document.querySelectorAll('.pending').length") == dots - 2
    tags = page.evaluate("""() => [...document.querySelectorAll('[data-box]')].map(b => [byId[b.getAttribute('data-box')].title,
        [...b.querySelectorAll(':scope > g > rect')].filter(r => r.getAttribute('fill') === COLOR.invalid).length]).filter(x => x[1])""")
    assert ["Greedy", 1] in tags
    run.click()                                                            # while busy: stop
    assert page.evaluate("window.posted.splice(0)") == [{"type": "chase-stop"}]
    page.evaluate("m => window.postMessage(m, '*')", {"type": "chase-busy", "on": False})
    page.wait_for_timeout(300)
    assert page.evaluate("document.querySelectorAll('.pending').length") == 0
    assert not errors, errors
    # not in an editor: no button
    plain, errors = open_page(browser, tmp_path, viz.from_spec(path), editor=False)
    assert not plain.locator("#run").is_visible() and not errors


def test_a_red_tag_opens_its_repairs(browser, tmp_path):
    # a click on the ⪯ that fails opens the panel by it (the boxes inside stay):
    # the constants, a repair asked of the editor for the part's constant, the
    # result shown, the change drawn on the diagram when pointed at, and Apply
    # asks the editor to write the value
    from test_viz import COUNT
    f = tmp_path / "count.ltl"
    f.write_text(COUNT)
    d = viz.from_spec(str(f), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"]["status"] = "invalid"
    page, errors = open_page(browser, tmp_path, d)
    tag = page.locator('[data-check$="|⪯"]').bounding_box()
    page.mouse.click(tag["x"] + tag["width"] / 2, tag["y"] + tag["height"] / 2)
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert page.evaluate("folded.size") == 0
    asked = page.evaluate("posted.filter(m => m.type === 'chase-repair')")
    assert len(asked) == 1           # the constant of the part at once; the requirement k only when picked
    assert asked[0]["request"]["check"] == "refines" and asked[0]["request"]["args"] == ["Counter", "Bound"]
    assert [q["name"] for q in asked[0]["request"]["params"]] == ["top"]
    assert page.locator("#repair input[type=checkbox]").evaluate_all("xs => xs.map(x => x.checked)") == [True, False]
    page.evaluate("""id => window.postMessage({ type: 'chase-repair-result', id, result: { status: 'repaired',
        method: 'counterexamples', rounds: 2, seconds: 0.1, values: { top: 1 }, text: { top: '1' }, cost: 2,
        optimal: true } }, '*')""", asked[0]["id"])
    page.wait_for_selector("#repair .rp-opt.repaired", timeout=5000)
    assert "top 3 → 1" in page.inner_text("#repair .rp-opt")
    page.locator("#repair .rp-opt.repaired").scroll_into_view_if_needed()
    card = page.locator("#repair .rp-opt.repaired").bounding_box()
    page.mouse.move(card["x"] + 20, card["y"] + 10)
    page.wait_for_timeout(100)
    assert page.locator("#view text[data-was]").all_text_contents() == ["top = 3 → 1"]
    assert page.locator("#view .pv").count() == 1          # the tag as it would be
    page.mouse.move(5, 400)
    page.wait_for_timeout(100)
    assert page.locator("#view text[data-was]").count() == 0
    page.locator("#repair button", has_text="Apply").scroll_into_view_if_needed()
    apply = page.locator("#repair button", has_text="Apply").bounding_box()
    page.mouse.click(apply["x"] + apply["width"] / 2, apply["y"] + apply["height"] / 2)
    edits = page.evaluate("posted.filter(m => m.type === 'chase-apply').map(m => m.edits)")
    assert edits == [[{"line": 4, "name": "top", "value": "1"}]]
    # the requirement: picked by hand, its repair asks twice before it is written
    page.locator("#repair input[type=checkbox]").nth(1).scroll_into_view_if_needed()
    box = page.locator("#repair input[type=checkbox]").nth(1).bounding_box()
    page.mouse.click(box["x"] + 5, box["y"] + 5)
    page.locator("#repair button", has_text="Find").scroll_into_view_if_needed()
    find = page.locator("#repair button", has_text="Find").bounding_box()
    page.mouse.click(find["x"] + 5, find["y"] + 5)
    asked = page.evaluate("posted.filter(m => m.type === 'chase-repair')")
    assert [q["name"] for q in asked[-1]["request"]["params"]] == ["top"]   # each alone, then together
    for i, (name, value) in enumerate([("top", "1"), ("k", "3"), (None, None)]):
        page.wait_for_function(f"posted.filter(m => m.type === 'chase-repair').length >= {i + 2}", timeout=5000)
        a = page.evaluate("posted.filter(m => m.type === 'chase-repair')")[i + 1]
        page.evaluate("""([id, name, value]) => window.postMessage({ type: 'chase-repair-result', id, result: name
            ? { status: 'repaired', method: 'projection', rounds: 1, seconds: 0.1, values: { [name]: +value },
                text: { [name]: value }, cost: 2, optimal: true }
            : { status: 'impossible', method: 'counterexamples', rounds: 3, seconds: 0.1 } }, '*')""",
                      [a["id"], name, value])
    page.wait_for_function("document.querySelectorAll('#repair .rp-opt').length === 3", timeout=5000)
    k = page.locator("#repair .rp-opt", has_text="k 1 → 3").locator("button", has_text="Apply")
    k.scroll_into_view_if_needed()
    b = k.bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    assert page.inner_text("#repair .rp-opt:has-text('k 1 → 3') button.warn") == "Change the requirement?"
    assert len(page.evaluate("posted.filter(m => m.type === 'chase-apply')")) == 1     # not yet
    page.locator("#repair .rp-opt:has-text('k 1 → 3') button.warn").scroll_into_view_if_needed()
    b = page.locator("#repair .rp-opt:has-text('k 1 → 3') button.warn").bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    assert page.evaluate("posted.filter(m => m.type === 'chase-apply').pop().edits") == [
        {"line": 12, "name": "k", "value": "3"}]
    none = page.inner_text("#repair .rp-opt.impossible")
    assert none.startswith("No value in") and "\ntop, k\n" in none
    page.keyboard.press("Escape")            # the details, back to the list
    assert page.is_visible("#repair .q-item") and "← Problems" not in page.inner_text("#insp-bar")
    page.keyboard.press("Escape")            # then the panel
    assert not page.is_visible("#repair")
    assert errors == []


@pytest.mark.skipif(not __import__("test_viz").HAS_MC, reason="nuXmv/NuSMV not installed")
def test_a_red_tag_says_why_it_fails(browser, tmp_path):
    # the panel of a refinement that fails: the counterexample, step by step,
    # with the loop; with no named constant, it says that no value can fix it
    spec = tmp_path / "w.ltl"
    spec.write_text("""NAME: w;
CONTRACT Counter:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        n = 0;
        G ((n < 3 -> n' = n + 1) & (n >= 3 -> n' = n));
CONTRACT Bound:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        G (n <= 1);
""")
    d = viz.from_spec(str(spec), refines=[("Counter", "Bound")], check=True)
    page, errors = open_page(browser, tmp_path, d)
    tag = page.locator('[data-check$="|⪯"]').bounding_box()
    page.mouse.click(tag["x"] + tag["width"] / 2, tag["y"] + tag["height"] / 2)
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    text = page.inner_text("#repair")
    assert "COUNTEREXAMPLE" in text.upper()
    assert "Does not hold" not in text         # the red tag says it
    steps = page.locator("#repair table.rp-trace tr").first.inner_text().split()
    assert steps[:2] == ["step", "0"] and any(s.startswith("↺") or s == "↺" for s in steps)
    assert "A number can be repaired once it is named" in text
    # the panel is docked: the view moves under it, the panel stays
    before = page.locator("#insp").bounding_box()
    page.mouse.move(200, 400); page.mouse.down(); page.mouse.move(320, 470, steps=6); page.mouse.up()
    assert page.locator("#insp").bounding_box() == before
    # the box of the check is marked, the term the counterexample breaks is red, its values on the ports
    assert page.locator("#view [data-box].pick").count() == 1
    assert page.locator("#view [data-box].pick rect[fill='#f6dcd9']").count() >= 1
    click(page, page.locator("#repair table.rp-trace td.k").last)    # a column of the table: its step on the diagram
    assert any(v.startswith("n = ") for v in page.locator("#view .pv-val").all_text_contents())
    # the list of the problems: this one, though it has no constant
    back = page.locator("#insp-bar button.back", has_text="Problems").bounding_box()
    page.mouse.click(back["x"] + back["width"] / 2, back["y"] + back["height"] / 2)
    assert "Counter does not refine Bound" in page.inner_text("#repair .q-item")
    assert page.locator("#insp-bar button").count() == 0       # nothing of the details left
    page.keyboard.press("Escape")
    assert not page.is_visible("#insp")
    assert errors == []


def test_the_steps_as_times(browser, tmp_path):
    # with a period, the counterexample is read in time: t = step x period
    spec = tmp_path / "w.ltl"
    spec.write_text("""NAME: w;
CONTRACT Counter:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        n = 0;
        G ((n < 3 -> n' = n + 1) & (n >= 3 -> n' = n));
CONTRACT Bound:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        G (n <= 1);
""")
    d = viz.from_spec(str(spec), refines=[("Counter", "Bound")], check=True, period="0.25")
    page, errors = open_page(browser, tmp_path, d)
    tag = page.locator('[data-check$="|⪯"]').bounding_box()
    page.mouse.click(tag["x"] + tag["width"] / 2, tag["y"] + tag["height"] / 2)
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    head = page.locator("#repair table.rp-trace tr").first.inner_text().split()
    assert head[:4] == ["t", "0", "0.25", "0.5"]
    assert page.get_attribute("#repair table.rp-trace th", "title") == "time: a step every 0.25 s"
    # a column says its time, and a click on it puts that time on the diagram
    cell = page.locator('#repair table.rp-trace td[data-k="2"]').first
    assert cell.get_attribute("title") == "Show t = 0.5 on the diagram"
    click(page, cell)
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "2"
    assert errors == []


def test_the_code_of_a_c_function(browser, tmp_path):
    # a component with a C function: its icon opens the code, with the lines of the file
    from chase_cps import CFunction, Component, Contract
    src = tmp_path / "f.c"
    src.write_text("int helper(int x) {\n    return x + 1;\n}\n\nint f(int a) { return helper(a); }\n")
    c = Contract.from_strings(inputs={"a": (0, 3)}, outputs={"b": (0, 9)}, guarantees=["b = a + 1"])
    d = viz.diagram(Component("F", c, CFunction(str(src), "f", result="b")))
    page, errors = open_page(browser, tmp_path, d)
    icon = page.locator("[data-box] .icon[data-tip*='its code']").bounding_box()
    page.mouse.click(icon["x"] + icon["width"] / 2, icon["y"] + icon["height"] / 2)
    page.wait_for_selector("#pop pre.code", timeout=5000)
    assert "f.c" in page.inner_text("#poptitle")
    codes = page.locator("#pop pre.code").all_inner_texts()
    assert codes[0].startswith("5") and "helper(a)" in codes[0]
    assert codes[1].startswith("1") and "return x + 1" in codes[1]
    # closed by a small cross, as the panel, or by Esc
    x = page.locator("#pop header button")
    assert x.get_attribute("aria-label") == "Close" and "ib" in x.get_attribute("class").split()
    assert x.locator("svg").count() == 1                    # the cross of the page, as an icon
    head, b = page.locator("#pop header").bounding_box(), x.bounding_box()
    assert abs((head["x"] + head["width"] - (b["x"] + b["width"])) - (b["y"] - head["y"])) <= 1.5   # right as top
    b = x.bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    assert not page.is_visible("#pop")
    page.mouse.click(icon["x"] + icon["width"] / 2, icon["y"] + icon["height"] / 2)
    page.wait_for_selector("#pop pre.code", timeout=5000)
    page.keyboard.press("Escape")
    assert not page.is_visible("#pop")
    assert errors == []


def test_the_button_of_the_transition_labels(browser, tmp_path):
    # shown only with a model drawn with its transitions; a model too large to
    # draw (no transitions) leaves the page as it is
    from chase_cps import Contract
    c = Contract.from_strings(name="C", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)"])
    for fsm, shown in [({"count": 5000, "too_many": True}, False),
                       ({"states": [{"id": "s0", "label": "s0", "init": True}, {"id": "s1", "label": "s1"}],
                         "edges": [{"from": "s0", "to": "s1", "label": "x"}], "count": 2}, True)]:
        d = viz.diagram(c)
        d["nodes"][0]["model"] = {"name": "m", "fsm": fsm}
        page, errors = open_page(browser, tmp_path, d)
        assert not errors
        assert page.evaluate("getComputedStyle(document.getElementById('conds')).display !== 'none'") == shown
        page.close()


def test_an_input_that_nothing_drives(browser, tmp_path):
    # in a decomposition drawn in the contract it refines: its name in the color of the warning,
    # a dashed stub before it, and no words over the names (a System draws the part not written instead)
    from chase_cps import Contract
    a = Contract.from_strings(name="A", inputs=["x", "q"], outputs=["y"], guarantees=["G ((x & q) -> y)"])
    b = Contract.from_strings(name="B", inputs=["y"], outputs=["z"], guarantees=["G (y -> z)"])
    top = Contract.from_strings(name="Top", inputs=["x"], outputs=["y", "z"], guarantees=["G (x -> z)"])
    ab = a.compose(b)
    page, errors = open_page(browser, tmp_path, viz.diagram(refines=[(ab, top, "valid")]))
    assert not errors
    q = page.locator('#view text[data-var="q"][data-side="left"]')
    assert q.count() == 1 and "warn" in (q.get_attribute("style") or "")
    assert "not driven" in q.get_attribute("data-tip")
    assert not page.evaluate("[...document.querySelectorAll('#view text')].some(t => t.textContent === 'not driven')")
    page.close()


def test_a_counterexample_of_one_state_repeated_forever(browser, tmp_path):
    # one state repeated forever is a table with its ↺; one call of a function is name = value
    from chase_cps import Contract
    c = Contract.from_strings(name="C", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)"])
    page, errors = open_page(browser, tmp_path, viz.diagram(c))
    lasso = page.evaluate("traceTable({vars: ['mode', 'speed'], steps: [[false, 120]], loop: 0}, [], 'C').outerHTML")
    assert "<table" in lasso and "↺" in lasso and "repeat forever" in lasso
    call = page.evaluate("traceTable({vars: ['x', 'y'], steps: [[1, 2]], loop: null}, [], 'C').outerHTML")
    assert "rp-values" in call and "x = 1" in call
    assert not errors
    page.close()


TWO_PROBLEMS = """NAME: w;
CONTRACT Counter:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        n = 0;
        G ((n < 3 -> n' = n + 1) & (n >= 3 -> n' = n));
CONTRACT Bound:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        G (n <= 1);
CONTRACT Inc:
    input integer (0:3) variable a;
    output integer (0:9) variable b;
    Assumptions:
        true;
    Guarantees:
        G (b = a + 1);
"""


def two_problems(browser, tmp_path):
    """A refinement that fails and a C function that does not implement its contract."""
    import shutil
    if not (__import__("test_viz").HAS_MC and (shutil.which("esbmc") or shutil.which("cbmc"))):
        pytest.skip("nuXmv and ESBMC or CBMC are needed")
    (tmp_path / "w.ltl").write_text(TWO_PROBLEMS)
    (tmp_path / "f.c").write_text("int inc(int a) {\n    return a + 2;\n}\n")
    d = viz.from_spec(str(tmp_path / "w.ltl"), refines=[("Counter", "Bound")],
                      implementations=[(str(tmp_path / "f.c") + ":inc", "Inc")], check=True)
    page, errors = open_page(browser, tmp_path, d)
    b = page.locator("#repairs").bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    page.wait_for_selector("#repair .q-item", timeout=5000)
    return page, errors


def click(page, locator):
    # the list is drawn again after the diagram (a few ms): the element is looked up again then
    for attempt in range(5):
        try:
            locator.scroll_into_view_if_needed(timeout=2000)
            b = locator.bounding_box()
            if b:
                break
        except pw.Error:
            page.wait_for_timeout(100)
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)


def test_the_panel_goes_from_the_list_to_the_details_and_back(browser, tmp_path):
    # no tabs: the list, the details of one check with "← Problems", and back, with nothing left of the details
    page, errors = two_problems(browser, tmp_path)
    assert page.inner_text("#insp-bar").split() == ["Problems", "2"]
    assert page.locator("#insp-bar button").count() == 0
    click(page, page.locator("#repair .q-item", has_text="implement"))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert page.inner_text("#insp-bar").strip() == "Problems"
    assert page.locator("#insp-bar button.back .ib svg").count() == 1   # the arrow of the page, as an icon
    # the code of the function: in the details, folded, after the counterexample
    code = page.locator("#repair details.rp-code")
    assert code.count() == 1 and not code.evaluate("d => d.open")
    assert page.inner_text("#repair details.rp-code summary").endswith("f.c: inc")
    order = page.evaluate("[...document.querySelectorAll('#repair > *')].map(e => e.className)")
    assert order.index("rp-sec rp-code") > [i for i, c in enumerate(order) if c == "rp-sec"][0]
    click(page, page.locator("#repair details.rp-code summary"))
    assert code.evaluate("d => d.open") and "return a + 2" in page.inner_text("#repair details.rp-code")
    # back to the list: the bar and the list as before
    click(page, page.locator("#insp-bar button.back"))
    assert page.inner_text("#insp-bar").split() == ["Problems", "2"]
    assert page.locator("#repair details.rp-code").count() == 0 and page.locator("#repair .q-item").count() == 2
    # Esc: the details, back to the list; then the panel
    click(page, page.locator("#repair .q-item", has_text="refine"))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    page.keyboard.press("Escape")
    assert page.locator("#repair .q-item").count() == 2 and page.locator("#insp-bar button").count() == 0
    page.keyboard.press("Escape")
    assert not page.is_visible("#insp")
    # a red tag on the diagram opens the details too, and goes back to the list
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    click(page, page.locator("#insp-bar button.back"))
    assert page.locator("#repair .q-item").count() == 2
    assert errors == []


def test_the_list_of_the_problems_is_aligned(browser, tmp_path):
    # the tags, of different widths, centered in a column of one width; the texts start at one place
    page, errors = two_problems(browser, tmp_path)
    tags = [page.locator("#repair .q-item .q-tag").nth(i).bounding_box() for i in range(2)]
    svgs = [page.locator("#repair .q-item .q-tag svg").nth(i).bounding_box() for i in range(2)]
    texts = [page.locator("#repair .q-item .q-main").nth(i).bounding_box() for i in range(2)]
    assert abs(tags[0]["width"] - tags[1]["width"]) < 0.5 and abs(tags[0]["x"] - tags[1]["x"]) < 0.5
    assert svgs[0]["width"] != svgs[1]["width"]             # the tags are not as wide
    for t, g in zip(tags, svgs):                             # each in the middle of the column
        assert abs((g["x"] + g["width"] / 2) - (t["x"] + t["width"] / 2)) < 1
    assert abs(texts[0]["x"] - texts[1]["x"]) < 0.5
    # the details keep the column: the title does not jump
    title = page.locator("#repair .q-item .t").first.bounding_box()
    click(page, page.locator("#repair .q-item").first)
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert abs(page.locator("#repair .rp-head b").bounding_box()["x"] - title["x"]) < 1
    assert errors == []


def test_the_step_on_the_diagram_in_the_table(browser, tmp_path):
    # the table picks the step on the diagram: a click on any cell of a column, or the arrow keys;
    # its column is blue, the column of the step that fails first stays red; no slider, no line about it
    page, errors = two_problems(browser, tmp_path)
    click(page, page.locator("#repair .q-item", has_text="refine"))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    fail = page.get_attribute("#repair table.rp-trace th.fail", "data-k")
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == fail       # it opens there
    assert page.locator("#repair .rp-step, #repair .rp-at, #repair input[type=range]").count() == 0
    cell = page.locator('#repair table.rp-trace td[data-k="0"]').first
    assert cell.get_attribute("title") == "Show step 0 on the diagram"
    click(page, cell)
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "0"
    assert page.locator('#repair table.rp-trace td.cur[data-k="0"]').count() >= 1
    assert page.get_attribute("#repair table.rp-trace th.fail", "data-k") == fail          # red stays
    assert "n = 0" in page.locator("#view .pv-val").all_text_contents()
    # the arrow keys, the panel focused: the next step and back
    page.keyboard.press("ArrowRight")
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "1"
    assert "n = 1" in page.locator("#view .pv-val").all_text_contents()
    page.keyboard.press("ArrowLeft"); page.keyboard.press("ArrowLeft")      # not before the first step
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "0"
    assert errors == []


def test_one_check_that_fails_opens_its_details(browser, tmp_path):
    # the wrench, with one check that fails: its details at once; back to the list of one
    (tmp_path / "w.ltl").write_text(TWO_PROBLEMS)
    d = viz.from_spec(str(tmp_path / "w.ltl"), refines=[("Counter", "Bound")], check=True)
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator("#repairs"))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert "does not refine" in page.inner_text("#repair .rp-head")
    click(page, page.locator("#insp-bar button.back"))
    assert page.locator("#repair .q-item").count() == 1
    assert errors == []


def test_the_icons_keep_their_distances(browser, tmp_path):
    # the cross of the model window as far from the right as from the top; the back arrow as far
    # from the left, the top, the bottom, and its word
    page, errors = two_problems(browser, tmp_path)
    click(page, page.locator("#repair .q-item", has_text="refine"))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    bar = page.locator("#insp-bar").bounding_box()
    icon = page.locator("#insp-bar .back .ib").bounding_box()
    word = page.evaluate("""() => { const r = document.createRange(), b = document.querySelector('#insp-bar .back');
      r.selectNodeContents(b.lastChild); return r.getBoundingClientRect().left; }""")
    left, top = icon["x"] - bar["x"], icon["y"] - bar["y"]
    bottom = bar["y"] + bar["height"] - 1 - (icon["y"] + icon["height"])       # less the line below the bar
    assert abs(left - top) <= 1.5 and abs(top - bottom) <= 1.5 and abs(word - (icon["x"] + icon["width"]) - left) <= 1.5
    assert errors == []
