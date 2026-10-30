"""The results of the checks of a specification file, kept next to it
(charger.ltl -> charger.results.json), so that a check is not run again
while what it reads has not changed.

A result is found by its key: a hash of what the check reads (the inputs,
the outputs, and the terms of the contracts, with their types; the text of a
model file), not of the names. A contract renamed keeps its results; a term
changed makes them go. The file is JSON written to be read: the result of
each contract under its name, and the results between contracts in a list.
"""

from __future__ import annotations

import datetime
import hashlib
import json
import os
import tempfile
from typing import Optional

from . import ltl
from .contract import Contract

#: The checks kept, and the part of the file they go in.
OF_A_CONTRACT = ("feasible",)
BETWEEN = ("refines", "implementation", "environment")


def results_path(spec: str) -> str:
    """Where the results of a specification file are kept."""
    base, _ = os.path.splitext(spec)
    return base + ".results.json"


def _text(c: Contract) -> str:
    """The contract as the checks read it: no name, the terms in any order."""
    d = c.to_dict()
    d.pop("name", None)
    for part in ("assumptions", "guarantees"):
        d[part] = sorted(d.get(part, []))
    return json.dumps(d, sort_keys=True)


def key(check: str, *parts) -> str:
    """The key of a check: a hash of the check, the version of CHASE, and
    what it reads (contracts, or the bytes of a file, or text)."""
    from . import __version__
    h = hashlib.sha256(f"{check}\n{__version__}\n".encode())
    for p in parts:
        if isinstance(p, Contract):
            h.update(_text(p).encode())
        elif isinstance(p, bytes):
            h.update(p)
        else:
            h.update(str(p).encode())
        h.update(b"\n")
    return h.hexdigest()[:16]


def model_bytes(path: str) -> bytes:
    """The text of a model file (FILE:MODULE reads FILE)."""
    plain = path.rsplit(":", 1)[0] if path.rsplit(":", 1)[0].endswith((".v", ".sv")) else path
    with open(plain, "rb") as f:
        return f.read() + path.encode()


class Record:
    """The results file of a specification: get and put results by key;
    save writes the file again (whole, then renamed into place)."""

    def __init__(self, path: str, spec: Optional[str] = None):
        self.path, self.spec = path, spec
        self.entries: dict[str, dict] = {}
        self.used: set[str] = set()
        self.changed = False
        try:
            with open(path, encoding="utf-8") as f:
                data = json.load(f)
        except (OSError, ValueError):
            return
        for name, checks in (data.get("contracts") or {}).items():
            for check, e in checks.items():
                if isinstance(e, dict) and "key" in e:
                    self.entries[e["key"]] = dict(e, check=check, of=name)
        for e in data.get("relations") or []:
            if isinstance(e, dict) and "key" in e:
                self.entries[e["key"]] = dict(e)

    def get(self, k: str) -> Optional[dict]:
        self.used.add(k)
        return self.entries.get(k)

    def put(self, k: str, check: str, result, **about) -> dict:
        """Keeps the result of a check (a Result, with its core when it
        fails); an unknown result is not kept: it is run again."""
        status = getattr(result, "status", None)
        if status is None or status == "unknown":
            return {}
        e = {"key": k, "check": check, **about, "result": status,
             "tool": getattr(result, "backend", "") or "",
             "seconds": round(getattr(result, "seconds", 0.0) or 0.0, 2),
             "at": datetime.datetime.now().isoformat(timespec="seconds")}
        core = getattr(result, "core", None)
        terms = getattr(core, "core", None)
        if isinstance(terms, Contract):
            e["core"] = {"assumptions": [ltl.to_str(t) for t in terms.assumptions],
                         "guarantees": [ltl.to_str(t) for t in terms.guarantees]}
        self.entries[k] = e
        self.used.add(k)
        self.changed = True
        return e

    def save(self, prune: bool = False) -> None:
        """Writes the file again; with prune, only the results used since
        it was read are kept (the others are of contracts changed since)."""
        if prune and set(self.entries) - self.used:
            self.entries = {k: e for k, e in self.entries.items() if k in self.used}
            self.changed = True
        if not self.changed:
            return
        from . import __version__
        contracts, relations = {}, []
        for e in self.entries.values():
            if e["check"] in OF_A_CONTRACT:
                body = {x: v for x, v in e.items() if x not in ("check", "of")}
                contracts.setdefault(e.get("of", "?"), {})[e["check"]] = body
            else:
                relations.append(e)
        data = {"file": os.path.basename(self.spec) if self.spec else None, "chase": __version__,
                "contracts": dict(sorted(contracts.items())),
                "relations": sorted(relations, key=lambda e: (e["check"], json.dumps(e, sort_keys=True)))}
        folder = os.path.dirname(os.path.abspath(self.path))
        fd, tmp = tempfile.mkstemp(prefix=".results-", dir=folder)
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
            f.write("\n")
        os.replace(tmp, self.path)
        self.changed = False


def as_result(e: dict):
    """A kept result, as the checks give it (with its core as the texts of
    its terms)."""
    from .queries import Result
    r = Result(status=e["result"], backend=e.get("tool", ""), seconds=e.get("seconds", 0.0),
               details={"kept": e.get("at", "")})
    if e.get("core"):
        r.core = (e["core"].get("assumptions", []), e["core"].get("guarantees", []))
    return r
