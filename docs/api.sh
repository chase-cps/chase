#!/bin/sh
# Writes the API reference of the chase_cps package (HTML) into the given
# directory, e.g. the api/ directory of the website repository:
#   docs/api.sh ../chase-cps.github.io/api
# Needs pdoc (pip install pdoc).
set -e
out=${1:?usage: docs/api.sh OUTPUT_DIR}
cd "$(dirname "$0")/.."
rm -rf "$out"
PYTHONPATH=python pdoc chase_cps --docformat restructuredtext \
    --footer-text "chase-cps $(grep -m1 '^version' pyproject.toml | cut -d'"' -f2)" -o "$out"
