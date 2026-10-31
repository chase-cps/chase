"""Back-end adapters and the dispatcher.

Each adapter declares the query types it answers; ``check(query)`` picks the
first available adapter answering the query, or the one named by ``backend``.
"""

from __future__ import annotations

import os

from ..queries import UNKNOWN, Result
from .base import Adapter
from .bmc import BMCAdapter
from .nusmv import NuSMVAdapter
from .slugs import SlugsAdapter
from .z3 import Z3Adapter

# Order matters: the first available adapter answering a query is used.
ADAPTERS: dict[str, Adapter] = {
    "nuxmv": NuSMVAdapter(tool="nuxmv"),
    "nusmv": NuSMVAdapter(tool="nusmv"),
    "slugs": SlugsAdapter(),
    "z3": Z3Adapter(),        # static formulas only; check() sends them there first
    "esbmc": BMCAdapter("esbmc"),     # C functions
    "cbmc": BMCAdapter("cbmc"),
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
    names the back end to use for the queries it answers. Otherwise a formula
    without temporal operators goes to z3 when it is installed (see
    ``chase_cps.backends.z3``). A back end may name
    its engine, as in "nuxmv/bdd" (see ``chase_cps.backends.nusmv``). A query with real
    variables goes through ``chase_cps.reals``; the others go straight to the
    back ends.
    """
    from .. import reals
    from ..queries import Holds
    from .z3 import why_not
    if backend == "auto" and not os.environ.get("CHASE_BACKEND") and "z3" in ADAPTERS \
            and isinstance(query, ADAPTERS["z3"].answers) and not why_not(query) \
            and all(hasattr(ADAPTERS["z3"], k) for k in options):
        # a formula without temporal operators: arithmetic, for z3
        return _configured(ADAPTERS["z3"], options).run(query)
    if not isinstance(query, Holds) and reals.has_real_variables(query):
        return reals.check(query, backend, **options)
    return dispatch(query, backend, **options)


def dispatch(query, backend: str = "auto", **options) -> Result:
    """``check`` without the handling of real variables."""
    preferred = os.environ.get("CHASE_BACKEND")
    if backend == "auto" and preferred and split(preferred)[0] in ADAPTERS \
            and isinstance(query, ADAPTERS[split(preferred)[0]].answers):
        backend = preferred
    backend, engine = split(backend)
    if engine:
        options = {**options, "engine": engine}
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
                  f"{type(query).__name__}; tried {candidates}. Run `chase-cps doctor` "
                  "to see how to install them")


def split(backend: str) -> tuple[str, str | None]:
    """"nuxmv/bdd" -> ("nuxmv", "bdd"): a back end and its engine, as the
    results name them."""
    name, _, engine = backend.partition("/")
    return name, engine or None


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


__all__ = ["ADAPTERS", "Adapter", "BMCAdapter", "NuSMVAdapter", "SlugsAdapter", "Z3Adapter", "check",
           "dispatch",
           "register", "adapters_for"]
