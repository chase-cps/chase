"""Access to the compiled CHASE modules (pychase, pychase_backends, ...).

The modules are looked up in this order: next to this package (wheel
installs), on ``sys.path``, then in ``$CHASE_INSTALL_DIR/chase/lib``.
"""

from __future__ import annotations

import importlib
import os
import sys

_here = os.path.dirname(os.path.abspath(__file__))
_native_dir = os.path.join(_here, "_lib")
if os.path.isdir(_native_dir) and _native_dir not in sys.path:
    sys.path.insert(0, _native_dir)


def _load(name: str):
    try:
        return importlib.import_module(name)
    except ImportError:
        prefix = os.environ.get("CHASE_INSTALL_DIR")
        if prefix:
            libdir = os.path.join(prefix, "chase", "lib")
            if libdir not in sys.path:
                sys.path.append(libdir)
            return importlib.import_module(name)
        raise ImportError(
            f"cannot import the CHASE module {name!r}: install CHASE "
            "(see https://chase-cps.github.io) and set CHASE_INSTALL_DIR "
            "or PYTHONPATH") from None


pychase = _load("pychase")
pychase_backends = _load("pychase_backends")
pychase_logicsLang = _load("pychase_logicsLang")

representation = pychase.representation
utilities = pychase.utilities
