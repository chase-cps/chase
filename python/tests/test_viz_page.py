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


@pytest.mark.parametrize("path", SPECS + SVA, ids=lambda f: os.path.relpath(f, DEMOS))
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
