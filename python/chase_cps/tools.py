"""The external tools of CHASE: where they are found, and their installation.

A tool is looked up in the ``PATH``, then in the ``bin`` directory of the
CHASE home (``$CHASE_HOME``, by default ``~/.chase``), where
``chase-cps install`` puts it. Each installation downloads a fixed version
from the tool's own site and checks its SHA-256 digest; slugs is built from
its sources.
"""

from __future__ import annotations

import hashlib
import os
import platform
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
import urllib.request

NUXMV_LICENSE = """\
nuXmv is distributed by Fondazione Bruno Kessler (FBK) under its own license:
it may be used only for academic, non-commercial research purposes, and it
may not be modified or redistributed. The full license is at
https://nuxmv.fbk.eu/license.html and in LICENSE.txt in the installed
directory."""

# (system, machine) -> (url, sha256, path of the executable in the archive)
NUXMV = {
    ("Linux", "x86_64"): (
        "https://nuxmv.fbk.eu/downloads/2.2.0/nuXmv-2.2.0-linux64.tar.xz",
        "d31709895ae2d7737e1e3c59eec9442c76527d2273380eaff6f3aea8cce9f946",
        "nuXmv-2.2.0-linux64/usr/local/bin/nuXmv"),
    ("Darwin", "arm64"): (
        "https://nuxmv.fbk.eu/downloads/2.2.0/nuXmv-2.2.0-macos64.tar.xz",
        "c552453e5e095c9181e7d800ba0fd5786752c25b7e16bff92c1ab2616f001d0d",
        "nuXmv-2.2.0-macos64/usr/local/bin/nuXmv"),
}
# NuSMV 2.6.0 is statically linked; the 2.7 builds need libedit.so.0, which
# Debian and Ubuntu do not have.
NUSMV = {
    ("Linux", "x86_64"): (
        "https://nusmv.fbk.eu/distrib/NuSMV-2.6.0-linux64.tar.gz",
        "11b12e42bc04566c4ec1f7cc205fa8704382ac4d69e052154cbb59c9b7e8e08c",
        "NuSMV-2.6.0-Linux/bin/NuSMV"),
}
SLUGS_COMMIT = "ac09bf652d4ff47a7018d6a41627af73a67a049c"
SLUGS = (f"https://github.com/VerifiableRobotics/slugs/archive/{SLUGS_COMMIT}.tar.gz",
         "383c452777c8d201b91630cc03e3a4f4f21794188d11418f0830f98da89b8b29")

# The macOS build of nuXmv links libraries of MacPorts (/opt/local/lib). They
# are taken from Homebrew instead, through DYLD_FALLBACK_LIBRARY_PATH, without
# modifying the binary (which its license forbids).
NUXMV_MACOS_LIBS = {"libxml2": "libxml2.16.dylib", "gmp": "libgmp.10.dylib",
                    "libedit": "libedit.0.dylib"}

# Shared libraries of the Linux builds -> (Debian/Ubuntu, Fedora/RHEL) packages.
LINUX_PACKAGES = {"libxml2.so.2": ("libxml2", "libxml2"), "libedit.so.2": ("libedit2", "libedit"),
                  "libgmp.so.10": ("libgmp10", "gmp")}

TOOLS = ("nuxmv", "nusmv", "slugs")


class InstallError(Exception):
    """The tool cannot be installed here; the message says what to do."""


def home() -> str:
    return os.path.expanduser(os.environ.get("CHASE_HOME", "~/.chase"))


def bin_dir() -> str:
    return os.path.join(home(), "bin")


def find(executable: str) -> str | None:
    """The path of an executable: a path as given, a name in the PATH, or a
    name in the bin directory of the CHASE home."""
    if os.sep in executable:
        return executable if os.path.isfile(executable) and os.access(executable, os.X_OK) else None
    found = shutil.which(executable)
    if found:
        return found
    path = os.path.join(bin_dir(), executable)
    return path if os.path.isfile(path) and os.access(path, os.X_OK) else None


def this_platform() -> tuple[str, str]:
    machine = platform.machine().lower()
    machine = {"amd64": "x86_64", "aarch64": "arm64" if sys.platform == "darwin" else "aarch64"}.get(machine, machine)
    return platform.system(), machine


def guideline(tool: str) -> str:
    """How to install the tool by hand."""
    return {
        "nuxmv": "Download nuXmv from https://nuxmv.fbk.eu/download.html, unpack it, and put "
                 "bin/nuXmv in the PATH or set CHASE_NUXMV to its path.",
        "nusmv": "Download NuSMV from https://nusmv.fbk.eu, or build it from its sources, "
                 "and put bin/NuSMV in the PATH or set CHASE_NUSMV to its path.",
        "slugs": "Build slugs from https://github.com/VerifiableRobotics/slugs (make in src/), "
                 "and put src/slugs in the PATH or set CHASE_SLUGS to its path.",
    }[tool] + " The CHASE Docker image (https://github.com/chase-cps/chase) contains the tools."


def installable(tool: str) -> bool:
    """Whether ``install(tool)`` supports this platform."""
    if tool == "nuxmv":
        return this_platform() in NUXMV
    if tool == "nusmv":
        return this_platform() in NUSMV
    return tool == "slugs" and platform.system() in ("Linux", "Darwin")


# Installation ------------------------------------------------------------------

def install(tool: str, accept_license: bool = False, log=print) -> str:
    """Install a tool in the CHASE home and return the path of its executable.

    nuXmv needs ``accept_license``. Raises InstallError when the tool cannot
    be installed here, with the way to install it by hand.
    """
    if tool not in TOOLS:
        raise InstallError(f"unknown tool {tool!r}; the tools are {', '.join(TOOLS)}")
    if tool == "slugs":
        return _install_slugs(log)
    table = NUXMV if tool == "nuxmv" else NUSMV
    system, machine = this_platform()
    if (system, machine) not in table:
        hint = (" On macOS, install nuXmv instead (chase-cps install nuxmv): the NuSMV builds "
                "for macOS need the libraries of MacPorts." if tool == "nusmv" and system == "Darwin" else "")
        raise InstallError(f"there is no {tool} build for {system} {machine}.{hint} {guideline(tool)}")
    if tool == "nuxmv" and not accept_license:
        raise InstallError(NUXMV_LICENSE + "\nTo accept the license and install nuXmv, run "
                           "chase-cps install nuxmv --accept-license")
    url, sha256, member = table[(system, machine)]
    libs = _homebrew_libs() if tool == "nuxmv" and system == "Darwin" else None
    root = _download_and_unpack(tool, url, sha256, log)
    executable = os.path.join(root, member)
    name = os.path.basename(executable)
    if system == "Linux":
        _check_shared_libraries(tool, executable)
    if libs:
        _write_wrapper(name, executable, libs)
    else:
        _link(name, executable)
    return _verify(tool, os.path.join(bin_dir(), name), log)


def _download(url: str, path: str, sha256: str, log) -> None:
    log(f"Downloading {url}")
    for attempt in range(4):
        try:
            with urllib.request.urlopen(url, timeout=60) as r, open(path, "wb") as f:
                shutil.copyfileobj(r, f)
            break
        except OSError as e:
            if attempt == 3:
                raise InstallError(f"cannot download {url}: {e}") from None
            time.sleep(5 * (attempt + 1))
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    if h.hexdigest() != sha256:
        raise InstallError(f"the SHA-256 digest of {url} is {h.hexdigest()}, expected {sha256}")


def _download_and_unpack(tool: str, url: str, sha256: str, log) -> str:
    """Unpack the archive into a fresh directory <home>/tools/<tool>."""
    dest = os.path.join(home(), "tools", tool)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    with tempfile.TemporaryDirectory(dir=os.path.dirname(dest)) as tmp:
        archive = os.path.join(tmp, "archive")
        _download(url, archive, sha256, log)
        unpacked = os.path.join(tmp, "unpacked")
        with tarfile.open(archive, "r:*") as tar:
            if hasattr(tarfile, "data_filter"):
                tar.extractall(unpacked, filter="data")
            else:
                for m in tar.getmembers():
                    if m.name.startswith("/") or ".." in m.name.split("/"):
                        raise InstallError(f"unsafe path {m.name!r} in {url}")
                tar.extractall(unpacked)
        if os.path.exists(dest):
            shutil.rmtree(dest)
        os.rename(unpacked, dest)
    log(f"Unpacked into {dest}")
    return dest


def _link(name: str, target: str) -> None:
    os.makedirs(bin_dir(), exist_ok=True)
    link = os.path.join(bin_dir(), name)
    if os.path.lexists(link):
        os.remove(link)
    os.symlink(target, link)


def _write_wrapper(name: str, target: str, libdirs: list[str]) -> None:
    os.makedirs(bin_dir(), exist_ok=True)
    path = os.path.join(bin_dir(), name)
    if os.path.lexists(path):
        os.remove(path)
    with open(path, "w") as f:
        f.write("#!/bin/sh\n"
                "# Written by chase-cps install: runs the binary of FBK with the\n"
                "# libraries of Homebrew in place of those of MacPorts.\n"
                f'DYLD_FALLBACK_LIBRARY_PATH="{":".join(libdirs)}'
                '${DYLD_FALLBACK_LIBRARY_PATH:+:$DYLD_FALLBACK_LIBRARY_PATH}" \\\n'
                f'    exec "{target}" "$@"\n')
    os.chmod(path, 0o755)


def _check_shared_libraries(tool: str, executable: str) -> None:
    ldd = shutil.which("ldd")
    if not ldd:
        return
    r = subprocess.run([ldd, executable], capture_output=True, text=True)
    output = (r.stdout + r.stderr).splitlines()
    old = sorted({line.split("version `")[1].split("'")[0] for line in output
                  if "version `" in line and "not found" in line})
    if old:
        raise InstallError(
            f"{tool} needs newer system libraries ({', '.join(old)}): the C++ library of "
            "GCC 13 or later, as in Ubuntu 24.04, Debian 13, or Fedora 39. On Ubuntu 22.04:\n"
            "  sudo apt-get install software-properties-common\n"
            "  sudo add-apt-repository ppa:ubuntu-toolchain-r/test\n"
            "  sudo apt-get install libstdc++6\n"
            f"Or use the CHASE Docker image. Then run chase-cps install {tool} again.")
    missing = sorted({line.split()[0] for line in output if "=> not found" in line})
    if not missing:
        return
    deb = " ".join(LINUX_PACKAGES.get(m, (m, m))[0] for m in missing)
    rpm = " ".join(LINUX_PACKAGES.get(m, (m, m))[1] for m in missing)
    raise InstallError(
        f"{tool} needs the libraries {', '.join(missing)}. Install them with\n"
        f"  sudo apt-get install {deb}    (Debian, Ubuntu)\n"
        f"  sudo dnf install {rpm}    (Fedora, RHEL)\n"
        f"and run chase-cps install {tool} again.")


def _homebrew_libs() -> list[str]:
    brew = shutil.which("brew")
    missing, dirs = [], []
    for formula, lib in NUXMV_MACOS_LIBS.items():
        prefix = None
        if brew:
            r = subprocess.run([brew, "--prefix", formula], capture_output=True, text=True)
            prefix = r.stdout.strip() if r.returncode == 0 else None
        libdir = os.path.join(prefix, "lib") if prefix else None
        if libdir and os.path.isfile(os.path.join(libdir, lib)):
            dirs.append(libdir)
        else:
            missing.append(formula)
    if missing:
        raise InstallError(
            "nuXmv for macOS needs the libraries " + ", ".join(NUXMV_MACOS_LIBS) + " of "
            "Homebrew (https://brew.sh). Install them with\n  brew install "
            + " ".join(NUXMV_MACOS_LIBS) + "\nand run chase-cps install nuxmv again.")
    return dirs


def _boost_include() -> str | None:
    candidates = [p for p in os.environ.get("CPATH", "").split(os.pathsep) if p]
    brew = shutil.which("brew")
    if brew:
        r = subprocess.run([brew, "--prefix", "boost"], capture_output=True, text=True)
        if r.returncode == 0:
            candidates.append(os.path.join(r.stdout.strip(), "include"))
    candidates += ["/opt/homebrew/include", "/usr/local/include", "/usr/include"]
    for c in candidates:
        if os.path.isfile(os.path.join(c, "boost", "version.hpp")):
            return c
    return None


def _install_slugs(log) -> str:
    if platform.system() not in ("Linux", "Darwin"):
        raise InstallError(f"slugs cannot be built here. {guideline('slugs')}")
    missing = [t for t in ("make", "c++") if not shutil.which(t)]
    if missing:
        raise InstallError("building slugs needs " + " and ".join(missing) + " (a C++ compiler "
                           "and make: Xcode command line tools on macOS, build-essential on Ubuntu)")
    boost = _boost_include()
    if not boost:
        raise InstallError("building slugs needs the Boost headers: brew install boost on "
                           "macOS, sudo apt-get install libboost-dev on Ubuntu")
    url, sha256 = SLUGS
    root = _download_and_unpack("slugs", url, sha256, log)
    src = os.path.join(root, f"slugs-{SLUGS_COMMIT}", "src")
    # The compiler options generator only knows x86_64 (as in the Dockerfile).
    gen = os.path.join(src, "BFAbstractionLibrary", "compilerOptionGenerator.c")
    with open(gen) as f:
        text = f.read()
    text = text.replace("#ifndef __x86_64", "#if !defined(__x86_64) && !defined(__aarch64__)")
    with open(gen, "w") as f:
        f.write(text)
    log("Building slugs (a few minutes)")
    env = dict(os.environ, CPATH=os.pathsep.join(p for p in (boost, os.environ.get("CPATH")) if p))
    r = subprocess.run(["make", "-j", str(os.cpu_count() or 2)], cwd=src, env=env,
                       capture_output=True, text=True)
    if r.returncode != 0 or not os.path.isfile(os.path.join(src, "slugs")):
        raise InstallError("building slugs failed:\n" + (r.stderr or r.stdout)[-2000:])
    _link("slugs", os.path.join(src, "slugs"))
    return _verify("slugs", os.path.join(bin_dir(), "slugs"), log)


def _verify(tool: str, path: str, log) -> str:
    """Run a small query with the installed tool."""
    from . import checks
    from .contract import Contract
    from .queries import REALIZABLE, VALID
    env = {"nuxmv": "CHASE_NUXMV", "nusmv": "CHASE_NUSMV", "slugs": "CHASE_SLUGS"}[tool]
    old = os.environ.get(env)
    os.environ[env] = path
    try:
        if tool == "slugs":
            c = Contract.from_strings(inputs=["r"], outputs=["g"], guarantees=["G (r -> X g)"],
                                      name="Check")
            r = checks.synthesize(c, "slugs", timeout=60)
            ok = r.status == REALIZABLE
        else:
            r = checks.implies("G a", "F a", {"a": "bool"}, tool, timeout=60)
            ok = r.status == VALID
    finally:
        if old is None:
            del os.environ[env]
        else:
            os.environ[env] = old
    if not ok:
        raise InstallError(f"{tool} was installed in {path}, but a test query failed: "
                           f"{r.status} {r.reason}")
    log(f"Installed {tool}: {path}")
    if not shutil.which(os.path.basename(path)):
        log(f"chase-cps finds it in {bin_dir()}; add this directory to the PATH to use it "
            "from the command line.")
    return path
