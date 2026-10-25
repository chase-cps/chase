#!/usr/bin/env bash
###############################################################################
# Builds and installs all the CHASE modules.
#
# Usage: ./install.sh [prefix]
#
#   prefix  installation directory (default: <repository>/install)
#
# Environment variables:
#   PYTHON          Python interpreter the Python modules are built for
#                   (default: python3)
#   pybind11_DIR    directory containing pybind11Config.cmake
#                   (default: detected)
#   BUILD_TYPE      CMake build type (default: Release)
#   BUILD_DIR       build directory (default: <repository>/build)
###############################################################################
set -euo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PREFIX=${1:-$ROOT/install}
BUILD_DIR=${BUILD_DIR:-$ROOT/build}
BUILD_TYPE=${BUILD_TYPE:-Release}

die() { echo "Error: $*" >&2; exit 1; }

for tool in git cmake; do
    command -v "$tool" >/dev/null 2>&1 || die "$tool is required."
done

# Absolute path of the prefix.
mkdir -p "$PREFIX"
PREFIX=$(cd "$PREFIX" && pwd)

# Sources of the modules.
if [ -e "$ROOT/.git" ]; then
    echo "=== Updating the git submodules"
    git -C "$ROOT" submodule update --init --recursive
fi
for m in third_party core-library chase-backends DSL_tool logics_tool; do
    [ -f "$ROOT/modules/$m/CMakeLists.txt" ] || \
        die "modules/$m is missing. Clone the repository with 'git clone --recursive'."
done

# Python interpreter.
PYTHON=${PYTHON:-python3}
PYTHON=$(command -v "$PYTHON") || die "Python 3 is required (set PYTHON)."
PY_VERSION=$("$PYTHON" -c 'import sys; print("%d.%d" % sys.version_info[:2])')

CMAKE_ARGS=(
    -DCMAKE_INSTALL_PREFIX="$PREFIX"
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DPython_EXECUTABLE="$PYTHON"
)

# pybind11.
if [ -n "${pybind11_DIR:-}" ]; then
    CMAKE_ARGS+=(-Dpybind11_DIR="$pybind11_DIR")
elif [ "$(uname -s)" = "Darwin" ] && command -v brew >/dev/null 2>&1; then
    CMAKE_ARGS+=(-DCMAKE_PREFIX_PATH="$(brew --prefix)")
elif [ -f /usr/lib/cmake/pybind11/pybind11Config.cmake ]; then
    CMAKE_ARGS+=(-Dpybind11_DIR=/usr/lib/cmake/pybind11)
elif dir=$("$PYTHON" -m pybind11 --cmakedir 2>/dev/null); then
    CMAKE_ARGS+=(-Dpybind11_DIR="$dir")
fi

echo "=== Building CHASE in $BUILD_DIR"
echo "    installation prefix: $PREFIX"
echo "    Python: $PYTHON ($PY_VERSION)"
cmake -S "$ROOT" -B "$BUILD_DIR" "${CMAKE_ARGS[@]}"
cmake --build "$BUILD_DIR"

cat <<EOF

CHASE has been installed in $PREFIX.
To use it, add the following lines to your shell configuration file:

    export PATH="$PREFIX/chase/bin:\$PATH"
    export PYTHONPATH="$PREFIX/chase/lib:\$PYTHONPATH"

The Python modules have been built for Python $PY_VERSION ($PYTHON).
EOF
