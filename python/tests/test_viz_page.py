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


def body_of(page, b, x, y):
    """A point of the body of box b, from (x, y) leftward and down, where no label of a wire lies over it."""
    return page.evaluate("""([id, x, y, w, h, x0, y0]) => {
        for (let dy = 0; y + dy < y0 + h - 4; dy += 6) for (let dx = 0; x - dx > x0 + 8; dx += 12) {
          const e = document.elementFromPoint(x - dx, y + dy), at = e && e.closest('[data-box]');
          if (at && at.getAttribute('data-box') === id && !e.closest('[data-line]')) return [x - dx, y + dy]; }
        return [x, y]; }""", [b["id"], x, y, b["w"], b["h"], b["x"], b["y"]])


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
        page.mouse.dblclick(*body_of(page, b, b["x"] + min(b["w"], 300) - 12, b["y"] + min(b["h"] - 6, 40)))
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
              {"check": "refines", "result": "valid", "refined": "Robot1+Robot2+Greedy", "abstract": "ChargingStation"}):
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
    APPLY = "#repair .rp-opt.repaired button[aria-label^='Write the new value']"
    assert page.get_attribute(APPLY, "data-tip") == "Write the new value into the file and check again"
    eye = page.locator("#repair .rp-opt.repaired button.ib").first
    assert eye.get_attribute("aria-label") == "Show this change on the diagram (the file is not changed)"
    assert "Change top" in page.inner_text("#repair .rp-opt.repaired")      # the card says which constant changes
    page.locator(APPLY).scroll_into_view_if_needed()
    apply = page.locator(APPLY).bounding_box()
    page.mouse.click(apply["x"] + apply["width"] / 2, apply["y"] + apply["height"] / 2)
    edits = page.evaluate("posted.filter(m => m.type === 'chase-apply').map(m => m.edits)")
    assert edits == [[{"line": 4, "name": "top", "value": "1"}]]
    # the requirement: picked by hand, its repair asks twice before it is written
    page.locator("#repair input[type=checkbox]").nth(1).scroll_into_view_if_needed()
    box = page.locator("#repair input[type=checkbox]").nth(1).bounding_box()
    page.mouse.click(box["x"] + 5, box["y"] + 5)
    FIND = "#repair button[aria-label^='Find']"
    page.locator(FIND).scroll_into_view_if_needed()
    find = page.locator(FIND).bounding_box()
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
    k = page.locator("#repair .rp-opt", has_text="k 1 → 3").locator("button[aria-label^='Write the new value']")
    assert "it changes a requirement" in k.get_attribute("aria-label")
    k.scroll_into_view_if_needed()
    b = k.bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    warn = "#repair .rp-opt:has-text('k 1 → 3') button.warn"
    assert page.get_attribute(warn, "aria-label") == "Click again to change the requirement"
    assert "click ✓ again to change the requirement" in page.inner_text("#repair .rp-opt:has-text('k 1 → 3')")
    assert len(page.evaluate("posted.filter(m => m.type === 'chase-apply')")) == 1     # not yet
    page.locator(warn).scroll_into_view_if_needed()
    b = page.locator(warn).bounding_box()
    page.mouse.click(b["x"] + b["width"] / 2, b["y"] + b["height"] / 2)
    assert page.evaluate("posted.filter(m => m.type === 'chase-apply').pop().edits") == [
        {"line": 12, "name": "k", "value": "3"}]
    none = page.inner_text("#repair .rp-opt.impossible")
    assert none.startswith("Change top and k together\nNo values of top in [") and "Edit the ranges above" in none
    assert page.locator("#repair button", has_text="Widen").count() == 0
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
    assert "To repair a number of the terms, name it in the file" in text      # G (n <= 1) has a number
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
    cell = page.locator('#repair table.rp-trace td[data-k="1"]').first
    assert cell.get_attribute("title") == "Show t = 0.25 on the diagram"
    click(page, cell)
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "1"
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


def test_one_call_of_a_c_function_is_its_values(browser, tmp_path):
    # the counterexample of a C function is one call: its inputs and outputs as name = value, each on
    # its line (In, Out), not a table of one step with "from this step on, the steps repeat forever"
    from chase_cps import CFunction, Component, Contract
    src = tmp_path / "f.c"
    src.write_text("int f(int a) { return a + 2; }\n")
    c = Contract.from_strings(inputs={"a": (0, 3)}, outputs={"b": (0, 9)}, guarantees=["b = a + 1"])
    d = viz.diagram(Component("F", c, CFunction(str(src), "f", result="b")))
    node, = [n for n in d["nodes"] if (n.get("model") or {}).get("source") == "c"]
    node["badge"] = {"text": "M ⊨ C", "status": "invalid", "why": {"violated": [0], "at": 0, "trace": {
        "vars": ["a", "b"], "more": 0, "steps": [[1, 3]], "loop": 0, "cut": False, "at": 0}}}
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|M ⊨ C"]'))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert page.locator("#repair .rp-io").inner_text().split() == ["In", "a", "=", "1", "Out", "b", "=", "3"]
    assert page.locator("#repair table.rp-trace").count() == 0
    assert "repeat forever" not in page.inner_text("#repair") and "↺" not in page.inner_text("#repair")
    assert page.locator(OPEN_WAVE).count() == 0
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
    assert page.inner_text("#repair details.rp-code summary").split() == ["CODE", "f.c", "·", "inc"]
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


def test_the_panel_keeps_a_whole_diagram_whole(browser, tmp_path):
    # a diagram seen whole when the panel opens is seen whole in the room left of the panel (fitted
    # to it if need be), and fitted to the whole window again when the panel closes; before, its left
    # went under the window by half the panel
    from test_viz import COUNT
    long = " & ".join(f"(n != {k} | X n != {k + 1})" for k in range(9))        # a box wider than tall
    (tmp_path / "count.ltl").write_text(COUNT + f"""CONTRACT Wide:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        G ({long});
""")
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")], open=["Wide"])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": MIXED["at"], "trace": MIXED})
    page, errors = open_page(browser, tmp_path, d)
    page.set_viewport_size({"width": 900, "height": 1400})        # the diagram fitted to the width
    click(page, page.locator('button[onclick="fit()"]'))
    def span():
        bs = [page.locator("[data-box]").nth(i).bounding_box() for i in range(page.locator("[data-box]").count())]
        bs = [b for b in bs if b]
        return min(b["x"] for b in bs), max(b["x"] + b["width"] for b in bs)
    left0, right0 = span()
    assert left0 >= 0 and right0 <= 900 and right0 - left0 > 900 - 424        # whole, wider than the room left
    click(page, page.locator("#repairs"))
    page.wait_for_selector("#insp", state="visible")
    left, right = span()
    assert left >= 0 and right <= page.locator("#insp").bounding_box()["x"]
    page.keyboard.press("Escape")                                  # the details, the list
    page.keyboard.press("Escape")                                  # the panel
    assert not page.is_visible("#insp")
    left, right = span()                                           # (its box opened by the details: larger)
    assert left >= 0 and right <= 900 and right - left > 900 - 424
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
    assert abs(page.locator("#repair .rp-head .t").bounding_box()["x"] - title["x"]) < 1
    assert errors == []


def test_the_titles_of_the_problems_name_the_parts_in_bold(browser, tmp_path):
    # a sentence in the words of the checks, with only the names of the parts, models and contracts
    # in bold; a second line with where the counterexample fails, or nothing; no file name
    page, errors = two_problems(browser, tmp_path)
    items = page.locator("#repair .q-item")
    texts = [items.nth(i).locator(".t").inner_text() for i in range(2)]
    bold = [items.nth(i).locator(".t b").all_inner_texts() for i in range(2)]
    assert "Counter does not refine Bound" in texts and "The C function inc does not implement Inc" in texts
    assert bold[texts.index("Counter does not refine Bound")] == ["Counter", "Bound"]
    assert bold[texts.index("The C function inc does not implement Inc")] == ["inc", "Inc"]
    assert page.evaluate("getComputedStyle(document.querySelector('#repair .q-item .t')).fontWeight") == "400"
    lines = page.locator("#repair .q-item .s").all_inner_texts()
    assert any(x.startswith("Fails at step 2 (n = 2)") for x in lines), lines
    assert not any("θ" in x or "no constant" in x or "f.c" in x for x in lines + texts)
    # the tooltip: the plain text, and what is found under it (a row shows one line of each)
    assert items.nth(0).get_attribute("title").split("\n")[0] == texts[0]
    assert items.nth(0).get_attribute("title").split("\n")[1].startswith("Fails at step 2")
    # the details: the same title, the names in bold
    click(page, items.filter(has_text="refine"))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    assert page.locator("#repair .rp-head .t b").all_inner_texts() == ["Counter", "Bound"]
    assert errors == []


def test_the_step_on_the_diagram_in_the_table(browser, tmp_path):
    # the table picks the step on the diagram: a click on any cell of a column, or the arrow keys;
    # it opens on the step that fails first (red), with no other mark; another step picked is blue;
    # no slider, no line about it
    page, errors = two_problems(browser, tmp_path)
    click(page, page.locator("#repair .q-item", has_text="refine"))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    fail = page.get_attribute("#repair table.rp-trace th.fail", "data-k")
    assert page.locator("#repair table.rp-trace .cur").count() == 0                     # it opens there
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
    click(page, page.locator(f'#repair table.rp-trace td[data-k="{fail}"]').first)   # back on the red: no blue
    assert page.locator("#repair table.rp-trace .cur").count() == 0
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


WIDE = """NAME: w;
CONTRACT Counter:
    output integer (0:20000) variable n;
    Assumptions:
        true;
    Guarantees:
        n = 1000;
        G ((n < 12000 -> n' = n + 1000) & (n >= 12000 -> n' = n));
CONTRACT Bound:
    output integer (0:20000) variable n;
    Assumptions:
        true;
    Guarantees:
        G (n <= 1000);
"""


@pytest.mark.skipif(not __import__("test_viz").HAS_MC, reason="nuXmv/NuSMV not installed")
def test_the_table_scrolls_under_its_first_column(browser, tmp_path):
    # a table wider than the panel: the names stay, with a page back and on above it; a table that
    # fits has none of these
    (tmp_path / "w.ltl").write_text(WIDE)
    d = viz.from_spec(str(tmp_path / "w.ltl"), refines=[("Counter", "Bound")], check=True)
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair .rp-trace-top.on", timeout=5000)
    left = lambda: page.evaluate("document.querySelector('#repair .rp-tracewrap').scrollLeft")
    assert page.locator("#repair .rp-span").count() == 0                # no line of the steps in view
    prev, nxt = page.locator("#repair .rp-trace-top button.ib").nth(0), page.locator("#repair .rp-trace-top button.ib").nth(1)
    assert prev.get_attribute("aria-label") == "Earlier steps" and nxt.get_attribute("aria-label") == "Later steps"
    assert prev.evaluate("b => getComputedStyle(b).visibility") == "hidden"
    assert nxt.evaluate("b => getComputedStyle(b).visibility") == "visible"
    name = page.locator("#repair table.rp-trace td.v").first
    x0 = name.bounding_box()["x"]
    click(page, nxt)
    page.wait_for_function("document.querySelector('#repair .rp-tracewrap').scrollLeft > 0", timeout=5000)
    assert left() > 0
    assert abs(name.bounding_box()["x"] - x0) < 1                    # the names stayed
    page.wait_for_function("getComputedStyle(document.querySelector('#repair .rp-trace-top button.ib'))"
                           ".visibility === 'visible'", timeout=5000)   # the scroll event comes a frame later
    for _ in range(10):                                              # on to the end: no page on
        if nxt.evaluate("b => getComputedStyle(b).visibility") == "hidden":
            break
        click(page, nxt)
    assert nxt.evaluate("b => getComputedStyle(b).visibility") == "hidden"
    assert errors == []


def test_the_table_has_a_scroll_bar_always_drawn(browser, tmp_path):
    # the bar of the system shows only while scrolling (macOS): the page draws one; its thumb drags the
    # table, a click on the bar pages it
    (tmp_path / "w.ltl").write_text(WIDE)
    d = viz.from_spec(str(tmp_path / "w.ltl"), refines=[("Counter", "Bound")], check=True)
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair .rp-bar.on", timeout=5000)
    wrap = "document.querySelector('#repair .rp-tracewrap').scrollLeft"
    thumb = page.locator("#repair .rp-bar > div").bounding_box()
    bar = page.locator("#repair .rp-bar").bounding_box()
    assert thumb["width"] < bar["width"]
    page.mouse.move(thumb["x"] + 4, thumb["y"] + 4); page.mouse.down()
    page.mouse.move(thumb["x"] + 64, thumb["y"] + 4, steps=4); page.mouse.up()
    assert page.evaluate(wrap) > 0
    assert page.locator("#repair .rp-bar > div").bounding_box()["x"] > thumb["x"]
    page.mouse.click(bar["x"] + 2, bar["y"] + 4)                     # left of the thumb: a page back
    page.wait_for_function(f"{wrap} === 0", timeout=5000)
    assert errors == []


def test_a_table_that_fits_has_no_scroll(browser, tmp_path):
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": 1, "trace": {
        "vars": ["n"], "steps": [[0], [1], [2]], "loop": 2, "at": 1, "more": 0, "cut": False}})
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    page.wait_for_timeout(100)
    assert page.locator("#repair .rp-trace-top.on").count() == 0
    assert not page.is_visible("#repair .rp-trace-top")
    assert not page.is_visible("#repair .rp-bar")
    assert errors == []


def test_a_repair_with_no_answer_can_try_longer(browser, tmp_path):
    # a repair stopped at the time limit says so, and asks again with twice the time
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"]["status"] = "invalid"
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair .rp-head", timeout=5000)
    page.wait_for_function("posted.some(m => m.type === 'chase-repair')", timeout=5000)   # asked after the layout
    asked = page.evaluate("posted.filter(m => m.type === 'chase-repair')")
    page.evaluate("""id => window.postMessage({ type: 'chase-repair-result', id, result: { status: 'unknown',
        method: 'counterexamples', rounds: 50, seconds: 60.2,
        reason: 'stopped at the time limit, before any value was found' } }, '*')""", asked[0]["id"])
    page.wait_for_selector("#repair .rp-opt.unknown", timeout=5000)
    card = page.inner_text("#repair .rp-opt.unknown")
    assert card.startswith("Change top\nNo answer within 60 s.") and "Not decided" not in card
    longer = page.locator("#repair .rp-opt.unknown button[aria-label^='Try longer']")
    assert longer.get_attribute("aria-label") == "Try longer: search again for up to 120 s"
    click(page, longer)
    again = page.evaluate("posted.filter(m => m.type === 'chase-repair')")
    assert len(again) == 2 and again[-1]["request"]["time_limit"] == 120
    assert [q["name"] for q in again[-1]["request"]["params"]] == ["top"]
    assert "Searching for up to 120 s" in page.inner_text("#repair")
    page.evaluate("""id => window.postMessage({ type: 'chase-repair-result', id, result: { status: 'repaired',
        method: 'counterexamples', rounds: 3, seconds: 70, values: { top: 1 }, text: { top: '1' }, cost: 2,
        optimal: true } }, '*')""", again[-1]["id"])
    page.wait_for_selector("#repair .rp-opt.repaired", timeout=5000)
    assert "top 3 → 1" in page.inner_text("#repair .rp-opt.repaired")
    assert errors == []


# The waveform of a counterexample: a drawer under the diagram, a row for each variable
def wave_page(browser, tmp_path, trace):
    """Bound fails with the given trace; the waveform opened with the mouse."""
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": trace["at"], "trace": trace})
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    return page, errors


OPEN_WAVE = '#insp-bar button.ib[aria-label^="Open the waveform"]'
MIXED = {"vars": ["go", "x", "n", "mode"], "more": 0, "cut": False, "loop": 3, "at": 2,
         "steps": [[True, 0.5, 0, "idle"], [False, 0.5, 1, "run"], [True, 1.25, 2, "run"], [None, 2, 3, "stop"], [True, 2, 3, "stop"]]}


def test_the_counterexample_is_at_the_same_place_in_every_problem(browser, tmp_path):
    # the heading and the table at the same height whatever is said besides: page buttons on the
    # line of the heading (a long trace), a sentence (the parts do not accept the input) under the table
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    tops = {}
    for name, trace, failed in [("short", MIXED, "guarantees"), ("long", LONG, "guarantees"), ("said", MIXED, "assumptions")]:
        d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
        bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
        bound["badge"].update(status="invalid", why={"failed": failed, "violated": [0], "at": trace["at"], "trace": trace})
        page, errors = open_page(browser, tmp_path, d)
        click(page, page.locator('[data-check$="|⪯"]'))
        page.wait_for_selector("#repair table.rp-trace", timeout=5000)
        tops[name] = page.evaluate("""() => { const P = document.getElementById('repair').getBoundingClientRect().top;
            const top = s => Math.round(document.querySelector(s).getBoundingClientRect().top - P);
            const t = document.querySelector('#repair table.rp-trace'), lead = document.querySelector('#repair .rp-lead');
            return [top('#repair .rp-sec .rp-h'), top('#repair table.rp-trace'), lead ? lead.getBoundingClientRect().top >= t.getBoundingClientRect().bottom : null,
                    Math.round(t.getBoundingClientRect().width) >= Math.round(t.parentNode.clientWidth) - 1]; }""")
        assert not errors
        page.close()
    assert tops["short"][:2] == tops["long"][:2] == tops["said"][:2]
    assert tops["said"][2] is True                       # the sentence under the table
    assert all(v[3] for v in tops.values())              # the table as wide as the panel


def test_in_a_narrow_window_the_waveform_takes_the_place_of_the_details(browser, tmp_path):
    # 800 pixels wide: the drawer where the details were, its arrow back to the problems; the table
    # writes the Booleans as the formulas do, ⊤ and ⊥
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": 2, "trace": MIXED})
    page, errors = open_page(browser, tmp_path, d)
    page.set_viewport_size({"width": 800, "height": 800})
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    assert "⊤" in page.inner_text("#repair table.rp-trace") and "⊥" in page.inner_text("#repair table.rp-trace")
    cells = page.eval_on_selector_all("#repair table.rp-trace td", "ts => ts.map(t => t.textContent)")
    assert "T" not in cells and "F" not in cells
    assert page.locator("#wave .wv-back").count() == 0
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    assert not page.is_visible("#insp") and page.is_visible("#wave .wv-back")
    box = page.locator("#wave").bounding_box()
    assert box["x"] == pytest.approx(12, abs=1) and box["x"] + box["width"] == pytest.approx(800 - 12, abs=1)
    assert box["y"] + box["height"] == pytest.approx(800 - 12, abs=1)
    click(page, page.locator("#wave .wv-back"))
    page.wait_for_selector("#repair .q-item", timeout=5000)
    assert not page.is_visible("#wave") and page.is_visible("#insp")
    # wide again: the arrow of the drawer is not there, the details are beside it
    page.set_viewport_size({"width": 1200, "height": 800})
    click(page, page.locator("#repair .q-item").first)
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    assert page.is_visible("#insp") and not page.is_visible("#wave .wv-back")
    assert not errors


def test_the_keys_stay_with_the_open_waveform(browser, tmp_path):
    # after a click on a button of the drawer that draws it again, or a click on the diagram, the keys
    # still move the cursor and zoom the drawer
    page, errors = wave_page(browser, tmp_path, MIXED)
    where = lambda: page.inner_text("#wave .wv-where")
    click(page, page.locator("#wave button.wv-plot").first)
    page.keyboard.press("ArrowLeft")
    assert where() == "Step 1"
    v = page.locator("#view").bounding_box()
    page.mouse.click(v["x"] + 30, v["y"] + 60)       # the background of the diagram
    page.keyboard.press("End")
    assert where() == "Step 4"
    page.keyboard.press("f")
    assert where() == "Step 2"
    px = page.evaluate("WAVE.px()")
    page.keyboard.press("-")
    assert page.evaluate("WAVE.px()") < px
    assert not errors


def wave_row(page, v):
    return page.locator("#wave .wv-row[data-i]", has=page.locator(f'.wv-name[title="{v}"]'))


def wave_step_x(page, k, v=None):
    """The middle of the column of step k in the first row of the waveform (or the row of v), on the screen."""
    svg = (wave_row(page, v) if v else page.locator("#wave .wv-row[data-i]").first).locator("svg.wv-svg").bounding_box()
    px = page.evaluate("WAVE.px()")
    return svg["x"] + (k + .5) * px, svg["y"] + svg["height"] / 2


def test_the_waveform_opens_and_closes(browser, tmp_path):
    # the button at the right of the line of the panel (Problems at its left); a drawer under the
    # diagram that leaves the details in view, with the check, a row for each variable (those of the
    # requirement first, under their group), the cross and Esc to close
    page, errors = wave_page(browser, tmp_path, MIXED)
    bar, b = page.locator("#insp-bar").bounding_box(), page.locator(OPEN_WAVE).bounding_box()
    assert "Problems" in page.inner_text("#insp-bar button.back")
    assert b["x"] + b["width"] == pytest.approx(bar["x"] + bar["width"] - 10, abs=2)
    assert not page.locator("#repair .wf-open").count()
    box, insp = page.locator("#wave").bounding_box(), page.locator("#insp").bounding_box()
    assert box["x"] == pytest.approx(12, abs=1) and box["y"] + box["height"] == pytest.approx(800 - 12, abs=1)
    assert box["y"] + box["height"] == pytest.approx(insp["y"] + insp["height"], abs=1)   # the bottoms on one line
    assert box["x"] + box["width"] == pytest.approx(insp["x"] - 12, abs=1)       # beside the details, not over them
    assert box["height"] <= 0.45 * 800 + 1
    assert "do not refine" in page.inner_text("#wave header b") or "does not refine" in page.inner_text("#wave header b")
    names = page.locator("#wave .wv-row[data-i] .wv-name").all_text_contents()
    assert names == ["n", "go", "x", "mode"]
    assert page.locator("#wave .wv-gname").all_inner_texts() == ["Variables of Bound", "Other variables"]
    page.keyboard.press("Escape")
    assert not page.is_visible("#wave")
    assert page.is_visible("#repair table.rp-trace")              # the details stay
    click(page, page.locator(OPEN_WAVE))
    click(page, page.locator('#wave button[aria-label="Close (Esc)"]'))
    assert not page.is_visible("#wave")
    assert errors == []


def test_the_diagram_ends_above_the_waveform(browser, tmp_path):
    # the drawer open: the diagram area ends above it, the diagram fitted there; closed: the whole window again
    page, errors = wave_page(browser, tmp_path, MIXED)
    top = page.locator("#wave").bounding_box()["y"]
    view = page.locator("#view").bounding_box()
    assert view["y"] + view["height"] <= top
    click(page, page.locator('button[onclick="fit()"]'))
    page.wait_for_timeout(100)
    boxes = [b for b in (page.locator(f"[data-box] >> nth={i}").bounding_box() for i in range(page.locator("[data-box]").count())) if b]
    assert max(b["y"] + b["height"] for b in boxes) <= top
    page.keyboard.press("Escape")
    view = page.locator("#view").bounding_box()
    assert view["y"] + view["height"] == pytest.approx(800, abs=1)
    assert errors == []


def test_a_tall_diagram_stays_readable_above_the_waveform(browser, tmp_path):
    # a diagram seen whole and tall: fitted above the drawer it would be too small to read (as LMFSM
    # was); its scale is kept and the box of the check brought above the drawer; at the close, the
    # view of before
    from test_viz import COUNT
    more = "".join(f"""CONTRACT Tall{j}:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
""" + "".join(f"        G (n != {k} | X n != {(k + j) % 9});\n" for k in range(8)) for j in range(4))
    (tmp_path / "count.ltl").write_text(COUNT + more)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")], open=[f"Tall{j}" for j in range(4)])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": MIXED["at"], "trace": MIXED})
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    click(page, page.locator('button[onclick="fit()"]'))
    page.wait_for_timeout(100)
    k0 = page.evaluate("cam.k")
    box = page.locator(f'[data-box="{bound["id"]}"] > rect')
    before = box.bounding_box()
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    page.wait_for_timeout(200)
    assert page.evaluate("fitScale()") < 0.7 * k0                  # the fit would have made it small
    assert page.evaluate("cam.k") >= 0.7 * k0
    r, top = box.bounding_box(), page.locator("#wave").bounding_box()["y"]
    assert r["y"] >= 0 and r["y"] + r["height"] <= top             # the box of the check above the drawer
    page.keyboard.press("Escape")
    page.wait_for_timeout(100)
    assert box.bounding_box() == pytest.approx(before, abs=1)
    assert errors == []


def test_the_waveform_has_no_group_heading_for_one_group(browser, tmp_path):
    # every variable in the terms of the requirement: no heading at all
    page, errors = wave_page(browser, tmp_path, {"vars": ["n"], "more": 0, "cut": False, "loop": 1, "at": 1,
                                                 "steps": [[0], [1]]})
    assert page.locator("#wave .wv-gname").count() == 0
    assert page.locator("#wave .wv-row[data-i]").count() == 1
    assert errors == []


def test_the_waveform_fits_its_columns(browser, tmp_path):
    # the names as wide as the longest (and the plot button), the values as the widest (as the table
    # writes them), the steps the rest of the width
    long = "a_rather_long_variable_name"
    trace = {"vars": [long, "h"], "more": 0, "cut": False, "loop": None, "at": 2,
             "steps": [[True, 1], [False, 17.96875], [True, 2.5]]}
    page, errors = wave_page(browser, tmp_path, trace)
    name = page.locator(f'#wave .wv-name[title="{long}"]')
    assert page.evaluate("e => e.scrollWidth <= e.clientWidth", name.element_handle())   # not cut
    left = page.locator("#wave .wv-row[data-i] .wv-left").first.bounding_box()["width"]
    assert left == page.evaluate("WAVE.left()") and left < 12 + 220 + 120
    assert set(wave_row(page, "h").locator("text.wv-v").all_text_contents()) == {"1", "17.96875", "2.5"}
    sc = page.locator("#wave .wv-scroll").bounding_box()
    assert page.evaluate("WAVE.px()") * 3 == pytest.approx(sc["width"] - left - 16, abs=1)   # all the steps in the width
    assert errors == []


def test_the_waveform_draws_each_kind_of_variable(browser, tmp_path):
    # a Boolean: a line high and low, filled under high; a number or a word: a box for each run of one
    # value, its value in it; no value: hatched; the loop drawn once more, faded; the step that fails
    # red with "fails"; a row of numbers as a line
    page, errors = wave_page(browser, tmp_path, MIXED)
    row = lambda v: wave_row(page, v)
    go = row("go")
    assert go.locator("path[stroke='#17393b']").count() == 2           # the steps, the faded pass
    assert go.locator("rect[fill='#c4d9d5']").count() >= 3
    assert go.locator("rect[fill='url(#wvhatch)']").count() == 2     # step 3: no value (and in the faded pass)
    x = row("x")
    assert {"0.5", "1.25", "2"} <= set(x.locator("text.wv-v").all_text_contents())
    assert x.locator("g[opacity='.35'] text").all_text_contents() == ["2"]    # the loop once more: step 3
    assert "stop" in row("mode").locator("text.wv-v").all_text_contents()
    assert row("go").locator("button.wv-plot").count() == 0
    click(page, row("n").locator("button.wv-plot"))
    n = row("n")
    assert n.locator("text.wv-v").count() == 0 and "max 3" in n.inner_text() and "min 0" in n.inner_text()
    assert n.locator("button.wv-plot.on").count() == 1
    assert n.bounding_box()["height"] > row("x").bounding_box()["height"]
    assert errors == []


def test_the_waveform_marks_the_loop_and_the_failure_on_their_lanes(browser, tmp_path):
    # the loop bracket on the top lane, "fails" under it: they never meet; the start of the loop a
    # dashed line down the rows; the faded pass labeled with the steps it repeats
    page, errors = wave_page(browser, tmp_path, MIXED)
    lp = page.locator("#wave .wv-axis text.wv-loop").bounding_box()
    fl = page.locator("#wave .wv-axis text.wv-fails").bounding_box()
    assert lp["y"] + lp["height"] <= fl["y"] + 1
    assert page.locator("#wave .wv-axis text.wv-tick.fail").text_content() == "2"
    assert page.locator("#wave .wv-axis text.wv-tick.ghost").all_text_contents() == ["3", "4", "…"]
    assert page.locator("#wave .wv-row[data-i] line[stroke-dasharray='3 3']").count() == 4
    assert "Steps 3 to 4 repeat forever; the faded columns are their next pass." in page.inner_text("#wave footer")
    assert errors == []


def test_the_waveform_shades_a_loop_from_the_first_step(browser, tmp_path):
    # a loop of all the steps (it starts at 0): shaded and dashed as any other
    trace = {"vars": ["go"], "more": 0, "cut": False, "loop": 0, "at": 1, "steps": [[True], [False]]}
    page, errors = wave_page(browser, tmp_path, trace)
    row = wave_row(page, "go")
    assert row.locator("rect[fill='#edf3f2']").count() == 1 and row.locator("line[stroke-dasharray='3 3']").count() == 1
    assert errors == []


def test_the_waveform_tells_the_value_under_the_pointer(browser, tmp_path):
    # the column under the pointer tinted, the value in full in the tip
    trace = {"vars": ["h"], "more": 0, "cut": False, "loop": None, "at": 1, "steps": [[1], [17.96875]]}
    page, errors = wave_page(browser, tmp_path, trace)
    page.mouse.move(*wave_step_x(page, 1, "h"))
    assert page.is_visible("#wave .wv-hov") and page.inner_text("#tip") == "h = 17.96875 · step 1"
    assert "17.96875" in wave_row(page, "h").locator("text.wv-v").all_text_contents()
    page.mouse.move(5, 5)
    assert not page.is_visible("#tip") and not page.is_visible("#wave .wv-hov")
    assert errors == []


def test_the_waveform_cursor_follows_the_table_and_the_diagram(browser, tmp_path):
    # a click on a step in the waveform: the cursor there, the table and the diagram on it; the arrows,
    # Home, End, F, L, the button Fails at, Alt and the arrows (the changes of the row) too; a step
    # picked in the table moves the cursor
    page, errors = wave_page(browser, tmp_path, MIXED)
    where = lambda: page.inner_text("#wave .wv-where")
    assert where() == "Step 2"                    # it opens on the step that fails
    page.mouse.click(*wave_step_x(page, 1))
    assert where() == "Step 1"
    assert page.get_attribute("#repair table.rp-trace th.cur", "data-k") == "1"
    assert "n = 1" in page.locator("#view .pv-val").all_text_contents()
    assert page.locator("#wave .wv-row[data-i] .wv-val").first.inner_text() == "1"
    for key, k in [("ArrowRight", 2), ("End", 4), ("Home", 0), ("l", 3), ("f", 2), ("ArrowLeft", 1)]:
        page.keyboard.press(key)
        assert where() == f"Step {k}", key
        assert page.evaluate("(document.querySelector('#repair table.rp-trace th.cur') || {}).dataset?.k ?? null") in (str(k), None)
    page.keyboard.press("Home")
    click(page, page.locator("#wave .wv-failat"))
    assert page.inner_text("#wave .wv-failat") == "Fails at 2" and where() == "Step 2"
    click(page, wave_row(page, "x").locator(".wv-name"))     # the row of x: 0.5 0.5 1.25 2 2
    page.keyboard.press("Home")
    for key, k in [("Alt+ArrowRight", 2), ("]", 3), ("]", 3), ("[", 2), ("Alt+ArrowLeft", 0)]:
        page.keyboard.press(key)
        assert where() == f"Step {k}", key
    page.evaluate("panel.goStep(2)")                  # the table moves: the cursor goes with it
    assert where() == "Step 2"
    page.mouse.click(*wave_step_x(page, 6))           # the faded pass: step 4 again is step 4 of the loop
    assert where() == "Step 4"
    assert errors == []


def test_the_waveform_keys_open_under_their_button(browser, tmp_path):
    # the keys: a box under the keyboard button (or over it), the drawer keeps its height
    page, errors = wave_page(browser, tmp_path, MIXED)
    h0 = page.locator("#wave").bounding_box()["height"]
    click(page, page.locator('#wave button[aria-label="Keys (?)"]'))
    keys, kb = page.locator("#wave .wv-keys").bounding_box(), page.locator('#wave button[aria-label="Keys (?)"]').bounding_box()
    assert page.is_visible("#wave .wv-keys") and page.locator("#wave").bounding_box()["height"] == h0
    assert keys["y"] >= kb["y"] + kb["height"] or keys["y"] + keys["height"] <= kb["y"]
    assert keys["x"] + keys["width"] == pytest.approx(kb["x"] + kb["width"], abs=1)
    assert 0 <= keys["y"] and keys["y"] + keys["height"] <= 800
    assert "The previous or next change of the selected row" in page.inner_text("#wave .wv-keys")
    assert page.inner_text("#wave .wv-keys-t") == "Keys of the waveform"
    caps = page.locator("#wave .wv-keys kbd").all_inner_texts()        # keycaps, one key each
    assert {"←", "→", "Home", "End", "F", "L", "Esc"} <= set(caps) and all(len(c) <= 5 for c in caps)
    page.keyboard.press("Escape")                     # Esc: the keys first, then the drawer
    assert not page.is_visible("#wave .wv-keys") and page.is_visible("#wave")
    assert errors == []


LONG = {"vars": ["n"] + [f"v{i}" for i in range(39)], "more": 0, "cut": False, "loop": 150, "at": 120,
        "steps": [[k % 10] + [(k // (i + 1)) % 3 == 0 if i % 2 else round(k * 0.1 * i, 2) for i in range(39)]
                  for k in range(200)]}


def test_the_markers_out_of_view_have_a_way_to_them(browser, tmp_path):
    # zoomed in at the start: "fails at 120 ▸" and "↺ loop from 150 ▸" at the right edge; a click goes there
    page, errors = wave_page(browser, tmp_path, LONG)
    for _ in range(3):
        page.keyboard.press("+")
    page.keyboard.press("Home")
    page.wait_for_timeout(100)
    right = page.locator("#wave .wv-edge.r button")
    assert right.all_inner_texts() == ["↺ loop from 150 ▸", "fails at 120 ▸"]   # each in the lane of its mark
    assert page.locator("#wave .wv-edge:not(.r) button").count() == 0
    click(page, right.last)
    page.wait_for_timeout(100)
    assert "Step 120" in page.inner_text("#wave header")
    assert "fails at 120" not in page.inner_text("#wave .wv-edge.r")
    # the loop's button on the first line, over nothing of "fails ▾" of the step in view (it was over it)
    loop_b = page.locator("#wave .wv-edge.r button").bounding_box()
    word = page.locator("#wave .wv-axis text.wv-fails").bounding_box()
    assert loop_b["y"] + loop_b["height"] <= word["y"] + 0.5
    page.keyboard.press("End")
    page.wait_for_timeout(100)
    left = page.locator("#wave .wv-edge:not(.r) button")
    assert left.all_inner_texts() == ["◂ ↺ loop from 150", "◂ fails at 120"]
    page.keyboard.press("l")                                       # the loop in view, the step that fails not:
                                                                   # its button on the second line still
    page.wait_for_timeout(100)
    assert left.all_inner_texts() == ["◂ fails at 120"]
    lane = page.locator("#wave .wv-axis text.wv-loop").bounding_box()
    assert left.bounding_box()["y"] >= lane["y"] + lane["height"] - 0.5
    assert errors == []


def test_another_problem_shows_its_waveform(browser, tmp_path):
    # the drawer open on one check, a click on the red tag of another: the drawer shows that one
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT + """CONTRACT Bound2:
    output integer (0:9) variable n;
    Assumptions:
        true;
    Guarantees:
        G (n <= 2);
""")
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound"), ("Counter", "Bound2")])
    for node, at in zip(d["nodes"], (2, 3)):
        node["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "at": at, "trace": MIXED})
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check="n1|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    assert page.inner_text("#wave header b").endswith("Bound")
    click(page, page.locator('[data-check="n2|⪯"]'))
    page.wait_for_function("() => document.querySelector('#wave header b') && document.querySelector('#wave header b').textContent.endsWith('Bound2')", timeout=5000)
    view = page.locator("#view").bounding_box()
    assert view["y"] + view["height"] <= page.locator("#wave").bounding_box()["y"]
    assert errors == []


def test_the_waveform_zooms_and_moves(browser, tmp_path):
    # + and -, Ctrl and the wheel where the pointer is (the step under it stays), 0 for all; a drag pans
    page, errors = wave_page(browser, tmp_path, LONG)
    sl = "document.querySelector('#wave .wv-scroll').scrollLeft"
    px, L = page.evaluate("WAVE.px()"), page.evaluate("WAVE.left()")
    assert px >= 32 and page.evaluate(sl) > 0            # too many steps to fit: the step that fails in view
    assert "steps " in page.inner_text("#wave footer .wv-span") and "of 200 in view" in page.inner_text("#wave footer .wv-span")
    click(page, page.locator('#wave button[aria-label="Zoom in (+)"]'))
    assert page.evaluate("WAVE.px()") == pytest.approx(px * 1.5)
    page.keyboard.press("-")
    assert page.evaluate("WAVE.px()") == pytest.approx(px)
    sc = page.locator("#wave .wv-scroll").bounding_box()
    x, y = sc["x"] + L + 200, sc["y"] + 80                # a step in the middle of the view
    under = lambda: page.evaluate(f"WAVE.stepAt({x})")
    k = under()
    assert k is not None
    page.mouse.move(x, y); page.keyboard.down("Control"); page.mouse.wheel(0, -100); page.keyboard.up("Control")
    page.wait_for_timeout(100)
    assert page.evaluate("WAVE.px()") > px and under() == k
    click(page, page.locator('#wave button[aria-label="Show all steps (0)"]'))
    assert page.evaluate("WAVE.px()") < 6 and page.evaluate(sl) == 0    # all the steps, below 6 pixels a step
    assert page.inner_text("#wave footer .wv-span").startswith("200 steps")
    page.keyboard.press("+"); page.keyboard.press("+"); page.keyboard.press("+")
    s0 = page.evaluate(sl)
    x, y = sc["x"] + L, sc["y"] + 80
    page.mouse.move(x + 300, y); page.mouse.down(); page.mouse.move(x + 100, y, steps=5); page.mouse.up()
    assert page.evaluate(sl) == pytest.approx(s0 + 200, abs=2)
    assert page.inner_text("#wave .wv-where") == "Step 120"     # a drag picks no step
    assert errors == []


def test_the_waveform_resizes_from_its_top_edge(browser, tmp_path):
    # many rows: as tall as 45% of the window goes, cut between two rows, the rows under it told;
    # dragged up, taller (by whole rows); a double-click: back to the first height
    page, errors = wave_page(browser, tmp_path, LONG)
    box = page.locator("#wave").bounding_box()
    assert 0.45 * 800 - 30 < box["height"] <= 0.45 * 800 + 1
    assert page.evaluate(CUT_ROW) is None
    assert page.is_visible("#wave .wv-fade") and "more rows below" in page.inner_text("#wave footer .wv-span")
    drag_grip(page, -150)
    tall = page.locator("#wave").bounding_box()
    assert tall["height"] == pytest.approx(box["height"] + 150, abs=26)
    assert tall["y"] + tall["height"] == pytest.approx(800 - 12, abs=1)
    assert page.evaluate(CUT_ROW) is None
    page.locator("#wave .wv-grip").dblclick()
    assert page.locator("#wave").bounding_box()["height"] == pytest.approx(box["height"], abs=1)
    assert errors == []


CUT_ROW = """(() => { const s = document.querySelector('#wave .wv-scroll'), end = s.scrollTop + s.clientHeight;
    const r = [...s.querySelectorAll('.wv-row')].find(r => r.offsetTop < end && r.offsetTop + r.offsetHeight > end + 1);
    return r ? r.dataset.i ?? 'axis' : null; })()"""


def drag_grip(page, dy):
    g = page.locator("#wave .wv-grip").bounding_box()
    page.mouse.move(g["x"] + g["width"] / 2, g["y"] + 3); page.mouse.down()
    page.mouse.move(g["x"] + g["width"] / 2, g["y"] + 3 + dy, steps=5); page.mouse.up()


def test_the_waveform_is_never_taller_than_its_rows(browser, tmp_path):
    # a few rows: dragged up, the drawer stops at its last row; the next drawer of more rows keeps
    # the height it was given, no blank under the rows
    page, errors = wave_page(browser, tmp_path, MIXED)
    h0 = page.locator("#wave").bounding_box()["height"]
    assert not page.is_visible("#wave .wv-fade")
    drag_grip(page, -300)
    assert page.locator("#wave").bounding_box()["height"] == pytest.approx(h0, abs=1)
    sc, grid = page.locator("#wave .wv-scroll").bounding_box(), page.locator("#wave .wv-grid").bounding_box()
    assert sc["height"] == pytest.approx(grid["height"], abs=1)
    assert errors == []


def test_the_waveform_fits_again_in_a_wider_window(browser, tmp_path):
    # all the steps in view: a wider window keeps them all, filling it; zoomed in, the zoom stays
    page, errors = wave_page(browser, tmp_path, MIXED)
    left = page.evaluate("WAVE.left()")
    width = lambda: page.locator("#wave .wv-scroll").bounding_box()["width"]
    page.set_viewport_size({"width": 1440, "height": 800})
    page.wait_for_timeout(150)
    assert page.evaluate("WAVE.px()") * 7 == pytest.approx(width() - left - 16, abs=1)   # 5 steps and 2 faded
    page.keyboard.press("+")
    px = page.evaluate("WAVE.px()")
    page.set_viewport_size({"width": 1280, "height": 800})
    page.wait_for_timeout(150)
    assert page.evaluate("WAVE.px()") == px
    assert errors == []


def test_the_waveform_zooms_around_the_cursor(browser, tmp_path):
    # + and the button zoom where the cursor is: its step stays in view
    page, errors = wave_page(browser, tmp_path, LONG)
    page.keyboard.press("End")
    in_view = """(() => { const s = document.querySelector('#wave .wv-scroll'), L = WAVE.left();
        const x = 199.5 * WAVE.px() - s.scrollLeft; return x > 0 && x < s.clientWidth - L - 16; })()"""
    assert page.evaluate(in_view)
    page.keyboard.press("+"); page.keyboard.press("+")
    assert page.evaluate(in_view)
    click(page, page.locator('#wave button[aria-label="Zoom in (+)"]'))
    assert page.evaluate(in_view) and page.inner_text("#wave .wv-where") == "Step 199"
    assert errors == []


def test_a_click_on_a_row_of_the_waveform_selects_it(browser, tmp_path):
    # a click on the wave of x: the step there and the row of x selected, for [ and ]
    page, errors = wave_page(browser, tmp_path, MIXED)
    page.mouse.click(*wave_step_x(page, 0, "x"))
    assert wave_row(page, "x").locator(".wv-left.sel").count() == 1
    page.keyboard.press("]")
    assert page.inner_text("#wave .wv-where") == "Step 2"     # x: 0.5 0.5 1.25: its next change at 2
    assert errors == []


def test_the_waveform_hint_tells_only_what_can_be_done(browser, tmp_path):
    # all the steps in view: no "drag to pan"; steps hidden: it says so; a narrow drawer: no hint
    page, errors = wave_page(browser, tmp_path, MIXED)
    hint = page.locator("#wave footer .wv-hint")
    page.set_viewport_size({"width": 1600, "height": 800})
    page.wait_for_timeout(150)
    mac = page.evaluate("/Mac|iPhone|iPad/.test(navigator.platform || navigator.userAgent)")
    assert "drag to pan" not in hint.inner_text() and ("⌘" if mac else "Ctrl") + " + wheel to zoom" in hint.inner_text()
    assert "Ctrl/⌘" not in hint.inner_text()              # the key of this computer, as in the keys
    page.keyboard.press("+"); page.keyboard.press("+")
    assert "drag to pan" in hint.inner_text()
    page.set_viewport_size({"width": 1100, "height": 800})
    page.wait_for_timeout(150)
    assert not hint.is_visible()
    assert errors == []


def test_the_table_pages_through_every_step(browser, tmp_path):
    # 200 steps: the table holds them all; it opens on the step that fails, a far step picked in the
    # waveform (the loop, L) comes into the middle of the table, and the page buttons reach the last step
    page, errors = wave_page(browser, tmp_path, LONG)
    wrap = page.locator("#repair .rp-tracewrap")
    def seen(k):
        w, c = wrap.bounding_box(), page.locator(f'#repair table.rp-trace th[data-k="{k}"]').bounding_box()
        return c["x"] >= w["x"] - 1 and c["x"] + c["width"] <= w["x"] + w["width"] + 1
    assert seen(120)
    page.keyboard.press("l")
    page.wait_for_timeout(100)
    assert seen(150) and not seen(120)
    nxt = page.locator('#repair button[aria-label="Later steps"]')
    for _ in range(40):
        if not nxt.is_visible() or seen(199):
            break
        click(page, nxt)
        page.wait_for_timeout(30)
    assert seen(199)
    assert errors == []


def test_an_environment_shows_the_step_and_the_assumption_it_breaks(browser, tmp_path):
    # the panel of E ⊨ A: the step where an assumption of the box it drives is seen to fail (red: req
    # drops at step 1 while grant is ⊥), and that assumption red on the box and listed in the panel
    page, errors = open_page(browser, tmp_path, environment_breaking()[0])
    click(page, page.locator('[data-check$="|E ⊨ A"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    assert page.locator("#repair table.rp-trace th.fail").inner_text() == "1"
    assert "The assumptions it breaks (red):" in page.inner_text("#repair")
    arb = page.evaluate("Object.values(byId).find(n => n.title === 'Arb').id")
    assert page.locator(f'[data-box="{arb}"] > g rect.core-row').count() >= 1
    assert "Fails at step 1" in page.evaluate("factsOf(byId[panel.id], 'E ⊨ A', null, byId[panel.id].badge.why)")
    assert errors == []


def environment_breaking(whole=False):
    """A contract Arb whose assumption an environment breaks, seen at step 1 (or, whole, over the whole run)."""
    from chase_cps import Contract, Model
    from chase_cps.queries import Result, Trace
    c = Contract.from_strings(name="Arb", inputs=["req"], outputs=["grant"],
                              assumptions=["F req" if whole else "G ((req & !grant) -> X req)"], guarantees=["G F grant"])
    env = Model.from_dict({"name": "Env", "inputs": {}, "outputs": {"req": "bool"}, "states": ["s"], "initial": "s",
                           "transitions": [{"from": "s", "outputs": {"req": False}, "to": "s"}]})
    run = [{"req": not whole, "grant": False}, {"req": False, "grant": False}]
    r = Result("invalid", trace=Trace(prefix=run, loop=[{"req": False, "grant": False}]))
    d = viz.diagram(c, environments=[(env, c, r)])
    return d, c


def test_an_environment_breaks_an_assumption_at_a_step_or_over_the_whole_run():
    for whole in (False, True):
        d, c = environment_breaking(whole)
        env, arb = d["nodes"]
        w = env["badge"]["why"]
        assert w["parts"] == {arb["id"]: [0]} and w["broken"][0].startswith("Arb: ")
        assert (w.get("at"), w["trace"].get("at"), w.get("whole")) == ((None, None, True) if whole else (1, 1, None))


def test_a_core_lists_its_terms_and_shows_each_on_the_diagram(browser, tmp_path):
    # an unrealizable contract: the panel says how many guarantees cannot all be met, lists them, and a
    # click on one flashes its lines on the diagram; How to fix is worded for realizability
    from chase_cps import Contract
    c = Contract.from_strings(name="R1", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)", "G (x -> !y)", "G F x"])
    d = viz.diagram(c)
    d["nodes"][0]["checks"] = [{"name": "R", "status": "invalid", "reason": "", "core": {"part": "contract", "A": [], "G": [0, 1]}}]
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|R"]'))
    page.wait_for_selector("#repair .rp-terms", timeout=5000)
    text = page.inner_text("#repair")
    assert "No component can meet both guarantees for every input that the assumptions allow" in text
    assert "The terms in conflict" not in text and "name it in the file" not in text   # no number in the terms
    terms = page.locator("#repair .rp-term")
    assert terms.count() == 2 and "x -> !y" in terms.nth(1).inner_text()
    assert page.locator("#repair .rp-term .f").first.evaluate("e => getComputedStyle(e).fontFamily").startswith("Menlo")
    click(page, terms.nth(1))
    page.wait_for_timeout(150)
    box = d["nodes"][0]["id"]
    assert page.locator(f'[data-box="{box}"] > g[data-term="contract:G1"] rect.flash').count() >= 1
    assert page.locator(f'[data-box="{box}"] > g[data-term="contract:G0"] rect.flash').count() == 0
    assert "Weaken a red guarantee" in text and "name it in the file" not in text     # no number in its terms
    assert "requirement" not in text.split("How to fix".upper())[-1].lower().replace("requires", "")
    assert errors == []


def test_the_terms_of_a_core_say_their_lines(browser, tmp_path):
    # terms cut short alike (a long computed contract) still tell apart: each says its line in the file
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"))
    n = next(n for n in d["nodes"] if n.get("source") and len((n["source"].get("G") or [])) >= 1)
    n["checks"] = [{"name": "R", "status": "invalid", "reason": "", "core": {"part": "contract", "A": [], "G": [0]}}]
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator(f'[data-check="{n["id"]}|R"]'))
    page.wait_for_selector("#repair .rp-terms", timeout=5000)
    assert page.inner_text("#repair .rp-term .ln") == "line " + str(n["source"]["G"][0][0])
    assert errors == []


def test_terms_that_begin_alike_show_where_they_differ(browser, tmp_path):
    # three long guarantees alike for their first 100 characters: each also shows the end that sets it apart
    from chase_cps import Contract
    head = "G ((" + " & ".join(f"state_{i}" for i in range(12)) + ") -> X counter = "
    c = Contract.from_strings(name="Long", inputs=["state_" + str(i) for i in range(12)], outputs=["counter"],
                              guarantees=[head + "0)", head + "1)", head + "2)"])
    d = viz.diagram(c)
    d["nodes"][0]["checks"] = [{"name": "R", "status": "invalid", "reason": "", "core": {"part": "contract", "A": [], "G": [0, 1, 2]}}]
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|R"]'))
    page.wait_for_selector("#repair .rp-terms", timeout=5000)
    ends = page.locator("#repair .rp-term .d").all_inner_texts()
    assert len(ends) == 3 and len(set(ends)) == 3 and all(e.startswith("… ") for e in ends)
    assert [e.rstrip(")").endswith(k) for e, k in zip(ends, "012")] == [True] * 3
    assert errors == []


def test_a_command_breaks_only_between_its_words(browser, tmp_path):
    # the repair command of a page that cannot run repairs, in a narrow panel: no flag cut at "--"
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["badge"]["status"] = "invalid"
    page, errors = open_page(browser, tmp_path, d, editor=False)
    page.set_viewport_size({"width": 900, "height": 800})
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair .rp-cmd", timeout=5000)
    spans = page.locator("#repair .rp-cmd > span")
    assert "--refines" in spans.all_inner_texts() and all(" " not in w for w in spans.all_inner_texts())
    assert page.evaluate("[...document.querySelectorAll('#repair .rp-cmd > span')].every(s => s.getClientRects().length === 1)")
    lines = page.evaluate("""(() => { const ys = new Set([...document.querySelectorAll('#repair .rp-cmd > span')]
                                       .map(s => Math.round(s.getBoundingClientRect().top))); return ys.size; })()""")
    assert lines > 1                                       # it wraps, at the spaces only
    assert errors == []


def test_back_to_problems_keeps_the_waveform_for_the_next(browser, tmp_path):
    # the waveform open, back to Problems, then a problem: it opens with its waveform
    page, errors = wave_page(browser, tmp_path, MIXED)
    click(page, page.locator("#insp-bar button.back"))
    page.wait_for_selector("#repair .q-item", timeout=5000)
    assert not page.is_visible("#wave")
    click(page, page.locator("#repair .q-item").first)
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    assert page.is_visible("#wave")
    page.keyboard.press("Escape")                          # the drawer closed: the next one opens without it
    page.wait_for_timeout(100)
    click(page, page.locator("#insp-bar button.back"))
    page.wait_for_selector("#repair .q-item", timeout=5000)
    click(page, page.locator("#repair .q-item").first)
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    assert not page.is_visible("#wave")
    assert errors == []


def test_the_waveform_of_a_long_trace_is_quick(browser, tmp_path):
    # 200 steps and 40 variables: the table and the waveform hold them all; opening it and a zoom are quick
    page, errors = wave_page(browser, tmp_path, LONG)
    assert page.locator("#repair table.rp-trace th[data-k]").count() == 200
    assert "The first" not in page.inner_text("#repair")
    assert page.locator("#wave .wv-row[data-i]").count() == 40
    page.keyboard.press("Escape")
    ms = page.evaluate("""(() => { const t = performance.now();
        document.querySelector('#insp-bar button.ib[aria-label^="Open the waveform"]').click();
        for (let i = 0; i < 5; i++) document.getElementById('wave').dispatchEvent(new KeyboardEvent('keydown', { key: '+' }));
        return (performance.now() - t) / 6; })()""")
    assert ms < 250, ms
    assert errors == []


def test_a_value_reads_in_the_waveform_as_in_the_table(browser, tmp_path):
    # the readout and the box write a number as the table does (17.875, not 17.88)
    trace = {"vars": ["h"], "more": 0, "cut": False, "loop": None, "at": 1, "steps": [[1], [17.875], [2]]}
    page, errors = wave_page(browser, tmp_path, trace)
    cell = page.locator('#repair table.rp-trace td[data-k="1"]').inner_text()
    assert cell == "17.875" and wave_row(page, "h").locator(".wv-val").inner_text() == cell
    assert "17.875" in wave_row(page, "h").locator("text.wv-v").all_text_contents()
    assert errors == []


def test_an_unknown_value_reads_the_same_in_the_table_and_the_waveform(browser, tmp_path):
    # "?" in the table cell (muted) as in the readout of the waveform, where the band is hatched
    page, errors = wave_page(browser, tmp_path, MIXED)
    td = page.locator("#repair table.rp-trace tr", has=page.locator("td.v", has_text="go")).locator('td[data-k="3"]')
    assert td.inner_text() == "?" and "unk" in td.get_attribute("class")
    click(page, page.locator('#repair table.rp-trace th[data-k="3"]'))
    assert wave_row(page, "go").locator(".wv-val").inner_text() == "?"
    assert wave_row(page, "go").locator("[fill='url(#wvhatch)']").count() >= 1
    assert errors == []


def test_a_requirement_broken_by_the_whole_run_marks_no_step(browser, tmp_path):
    # F n = 9 fails on the whole run, its loop included: no column red, no "fails" in the drawer, and
    # the details and the footer say so in words
    from test_viz import COUNT
    (tmp_path / "count.ltl").write_text(COUNT)
    d = viz.from_spec(str(tmp_path / "count.ltl"), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    bound["contract"]["G"] = ["F n = 9"]
    trace = {"vars": ["n"], "more": 0, "cut": False, "loop": 3, "steps": [[0], [1], [2], [3]]}   # no step, as _why gives it
    bound["badge"].update(status="invalid", why={"failed": "guarantees", "violated": [0], "trace": trace})
    page, errors = open_page(browser, tmp_path, d)
    click(page, page.locator('[data-check$="|⪯"]'))
    page.wait_for_selector("#repair table.rp-trace", timeout=5000)
    assert page.locator("#repair table.rp-trace .fail").count() == 0
    assert "No single step breaks Bound: the whole run does, its loop repeated forever." in page.inner_text("#repair")
    click(page, page.locator(OPEN_WAVE))
    page.wait_for_selector("#wave .wv-row", timeout=5000)
    assert page.locator("#wave .wv-failat").count() == 0 and "fails" not in page.inner_text("#wave").lower()
    assert page.locator("#wave rect[fill='#fbeceb']").count() == 0
    assert "No single step breaks Bound: the whole run does." in page.inner_text("#wave footer")
    assert errors == []


def test_a_value_label_sits_beside_the_cursor(browser, tmp_path):
    # the cursor inside a long run of one value, at its middle: the word in the larger part beside the
    # line, not on it
    trace = {"vars": ["x"], "more": 0, "cut": False, "loop": None, "at": 4,
             "steps": [[7], [7], [7], [7], [7], [7], [7], [7], [2]]}
    page, errors = wave_page(browser, tmp_path, trace)
    row = wave_row(page, "x")
    line = row.locator("line[stroke='#2f6fb5']").bounding_box()
    word = row.locator("text.wv-v", has_text="7").bounding_box()
    assert word["x"] + word["width"] < line["x"]                       # left of the line, the larger part
    for _ in range(3):
        page.keyboard.press("ArrowLeft")
    line = row.locator("line[stroke='#2f6fb5']").bounding_box()          # at step 1: the larger part is right
    word = row.locator("text.wv-v", has_text="7").bounding_box()
    assert word["x"] > line["x"] + line["width"]
    assert errors == []


def wave_middle(page, k):
    """How far the middle of the column of step k is from the middle of the steps in view, in pixels."""
    return page.evaluate(f"""(() => {{ const s = document.querySelector('#wave .wv-scroll'), L = WAVE.left(), px = WAVE.px();
      return ({k} + .5) * px - (s.scrollLeft + (Math.max(120, s.clientWidth - L - 16)) / 2); }})()""")


def test_a_long_waveform_opens_with_its_values_whole_and_no_stub_words(browser, tmp_path):
    # at first a step wide enough for most values (not "1…"), the step that fails in the middle; zoomed
    # out until two characters would fit, a box shows no word rather than "2…"
    page, errors = wave_page(browser, tmp_path, LONG)
    words = lambda: [t for t in page.locator("#wave .wv-v").all_text_contents() if t]
    shown = words()
    assert shown and sum(t.endswith("…") for t in shown) <= 0.2 * len(shown)
    assert abs(wave_middle(page, 120)) <= 1
    page.keyboard.press("0")
    for _ in range(5):
        page.keyboard.press("+")
    page.wait_for_timeout(100)
    assert not [t for t in words() if t.endswith("…") and len(t) < 4]
    assert errors == []


def test_all_the_steps_fit_and_narrow_steps_are_drawn_as_bands(browser, tmp_path):
    # 0: all 200 steps (and the faded pass) in the width; a number row a band with a tick at each change,
    # no boxes and no words; - can go no further and says so, + can
    page, errors = wave_page(browser, tmp_path, LONG)
    page.keyboard.press("0")
    page.wait_for_timeout(100)
    sc = "document.querySelector('#wave .wv-scroll')"
    assert page.evaluate(f"{sc}.scrollWidth <= {sc}.clientWidth + 1")
    assert page.inner_text("#wave footer .wv-span").startswith("200 steps")
    row = wave_row(page, "v0").locator("svg.wv-svg")
    assert row.locator("path").count() <= 3 and row.locator("text").count() == 0
    assert "off" in page.locator('#wave button[aria-label="Zoom out (-)"]').get_attribute("class")
    assert "off" not in page.locator('#wave button[aria-label="Zoom in (+)"]').get_attribute("class")
    px = page.evaluate("WAVE.px()")
    page.keyboard.press("-")
    assert page.evaluate("WAVE.px()") == px
    assert errors == []


def test_a_fitted_waveform_stays_fitted_as_the_window_changes(browser, tmp_path):
    # all the steps in view at 1440: narrower, then wider, all of them still; after a zoom, the zoom stays
    page, errors = wave_page(browser, tmp_path, MIXED)
    sc = "document.querySelector('#wave .wv-scroll')"
    fits = lambda: page.evaluate(f"{sc}.scrollWidth <= {sc}.clientWidth + 1")
    page.set_viewport_size({"width": 1440, "height": 800}); page.wait_for_timeout(150)
    wide = page.evaluate("WAVE.px()")
    assert fits()
    page.set_viewport_size({"width": 1100, "height": 800}); page.wait_for_timeout(150)
    assert fits() and page.evaluate("WAVE.px()") < wide
    page.set_viewport_size({"width": 1440, "height": 800}); page.wait_for_timeout(150)
    assert fits() and page.evaluate("WAVE.px()") == pytest.approx(wide, abs=0.5)
    page.keyboard.press("+")
    zoomed = page.evaluate("WAVE.px()")
    page.set_viewport_size({"width": 1100, "height": 800}); page.wait_for_timeout(150)
    assert page.evaluate("WAVE.px()") == zoomed
    assert errors == []


def test_the_selected_row_stays_in_view_and_is_tinted(browser, tmp_path):
    # forty rows, the drawer shows some: down past the bottom, the drawer scrolls to keep the row in view
    page, errors = wave_page(browser, tmp_path, LONG)
    for _ in range(25):
        page.keyboard.press("ArrowDown")
    page.wait_for_timeout(100)
    sel = page.locator("#wave .wv-row.sel")
    assert sel.count() == 1 and sel.locator(".wv-left.sel").count() == 1
    r, box = sel.bounding_box(), page.locator("#wave .wv-scroll").bounding_box()
    assert r["y"] >= box["y"] + 54 - 0.5 and r["y"] + r["height"] <= box["y"] + box["height"] + 0.5
    assert page.evaluate("getComputedStyle(document.querySelector('#wave .wv-row.sel')).backgroundColor") != "rgba(0, 0, 0, 0)"
    for _ in range(25):
        page.keyboard.press("ArrowUp")
    page.wait_for_timeout(100)
    r, box = page.locator("#wave .wv-row.sel").bounding_box(), page.locator("#wave .wv-scroll").bounding_box()
    assert r["y"] >= box["y"] + 54 - 0.5
    assert errors == []


def test_a_jump_brings_its_step_to_the_middle(browser, tmp_path):
    # F, L and the buttons of the markers put a step from out of view in the middle; an arrow moves as
    # little as it can
    page, errors = wave_page(browser, tmp_path, LONG)
    sl = "document.querySelector('#wave .wv-scroll').scrollLeft"
    page.keyboard.press("+"); page.keyboard.press("Home")
    page.keyboard.press("f")
    page.wait_for_timeout(100)
    assert abs(wave_middle(page, 120)) <= 1
    s0 = page.evaluate(sl)
    page.keyboard.press("ArrowRight")
    assert page.evaluate(sl) == s0                         # 121 was in view: no scroll
    page.keyboard.press("l")
    page.wait_for_timeout(100)
    assert abs(wave_middle(page, 150)) <= 1
    page.keyboard.press("Home")
    page.wait_for_timeout(100)
    click(page, page.locator("#wave .wv-edge.r button", has_text="fails at 120"))
    page.wait_for_timeout(100)
    assert abs(wave_middle(page, 120)) <= 1
    assert errors == []


def test_the_waveform_edges_and_marks(browser, tmp_path):
    # the first box of a row flat on its left (nothing changes before step 0); the header not selectable
    # and its top edge 12 pixels to grab; "fails" kept in the part in view when its column is half out
    page, errors = wave_page(browser, tmp_path, MIXED)
    d = wave_row(page, "n").locator("svg.wv-svg path").first.get_attribute("d")
    assert d.startswith("M0.5 ") and "L0.5 4" in d
    assert page.evaluate("getComputedStyle(document.querySelector('#wave header')).userSelect") == "none"
    assert page.locator("#wave .wv-grip").bounding_box()["height"] == 12
    page.close()
    page, errors = wave_page(browser, tmp_path, LONG)
    for _ in range(3):
        page.keyboard.press("+")
    px, L = page.evaluate("WAVE.px()"), page.evaluate("WAVE.left()")
    page.evaluate(f"document.querySelector('#wave .wv-scroll').scrollLeft = {120 * px + 0.6 * px}")
    page.wait_for_timeout(100)
    word, sc = page.locator("#wave .wv-axis text.wv-fails").bounding_box(), page.locator("#wave .wv-scroll").bounding_box()
    assert word["x"] >= sc["x"] + L - 0.5
    assert errors == []
