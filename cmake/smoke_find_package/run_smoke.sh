#!/usr/bin/env sh
# Build awtk-mvvm, install to a prefix, then configure this smoke project.
# Usage:
#   AWTK_PREFIX=/path/to/awtk-install ./run_smoke.sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
if [ -z "${AWTK_PREFIX:-}" ]; then
  echo "Set AWTK_PREFIX to the CMake install prefix of AWTK (contains lib/cmake/awtk)." >&2
  exit 1
fi
BUILD="${ROOT}/build-cmake"
PREFIX="${BUILD}/prefix-smoke"

cmake -S "$ROOT" -B "$BUILD" \
  -DCMAKE_PREFIX_PATH="$AWTK_PREFIX" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD" -j8
cmake --install "$BUILD" --prefix "$PREFIX"

cmake -S "$(dirname "$0")" -B "${BUILD}/smoke-find-package" \
  -DCMAKE_PREFIX_PATH="${PREFIX};${AWTK_PREFIX}" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build "${BUILD}/smoke-find-package" -j8

echo "OK: smoke_find_package linked against installed awtk-mvvm."
