"""Back-end adapters and the dispatcher.

Each adapter declares the query types it answers; ``check(query)`` picks the
first available adapter answering the query, or the one named by ``backend``.
"""

from __future__ import annotations

import os

from ..queries import UNKNOWN, Result
from .base import Adapter
from .nusmv import NuSMVAdapter
from .slugs import SlugsAdapter

# Order matters: the first available adapter answering a query is used.
ADAPTERS: dict[str, Adapter] = {
    "nuxmv": NuSMVAdapter(tool="nuxmv"),
    "nusmv": NuSMVAdapter(tool="nusmv"),
    "slugs": SlugsAdapter(),
}


def register(name: str, adapter: Adapter, first: bool = False) -> None:
    """Add an adapter (e.g. for another model checker)."""
    global ADAPTERS
    if first:
        ADAPTERS = {name: adapter, **{k: v for k, v in ADAPTERS.items() if k != name}}
    else:
        ADAPTERS[name] = adapter


def adapters_for(query) -> list[str]:
    return [n for n, a in ADAPTERS.items() if isinstance(query, a.answers)]


def check(query, backend: str = "auto", **options) -> Result:
    """Answer a query with the given back end, or the first available one.

    With backend "auto", the environment variable CHASE_BACKEND, when set,
    names the back end to use for the queries it answers.
    """
    preferred = os.environ.get("CHASE_BACKEND")
    if backend == "auto" and preferred and preferred in ADAPTERS \
            and isinstance(query, ADAPTERS[preferred].answers):
        backend = preferred
    if backend != "auto":
        if backend not in ADAPTERS:
            raise ValueError(f"unknown back end {backend!r}; known: {sorted(ADAPTERS)}")
        adapter = ADAPTERS[backend]
        if not isinstance(query, adapter.answers):
            return Result(UNKNOWN, backend,
                          reason=f"{backend} does not answer {type(query).__name__}")
        return _configured(adapter, options).run(query)
    candidates = adapters_for(query)
    if not candidates:
        return Result(UNKNOWN, reason=f"no back end answers {type(query).__name__} yet")
    for name in candidates:
        if ADAPTERS[name].available():
            return _configured(ADAPTERS[name], options).run(query)
    return Result(UNKNOWN, reason="no installed back end answers "
                  f"{type(query).__name__}; tried {candidates}")


def _configured(adapter: Adapter, options: dict) -> Adapter:
    if not options:
        return adapter
    import copy
    a = copy.copy(adapter)
    for k, v in options.items():
        if not hasattr(a, k):
            raise TypeError(f"{adapter.name} has no option {k!r}")
        setattr(a, k, v)
    return a


__all__ = ["ADAPTERS", "Adapter", "NuSMVAdapter", "SlugsAdapter", "check",
           "register", "adapters_for"]
