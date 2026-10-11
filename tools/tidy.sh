#!/usr/bin/env bash
# Run clang-tidy over our sources, using the compile commands of a configured
# build (default: build/dev; run `cmake --preset dev` first).
set -euo pipefail
cd "$(dirname "$0")/.."

build_dir="${1:-build/dev}"
extra=()
# Apple's compiler finds the SDK implicitly; clang-tidy has to be told.
if [[ "$(uname)" == "Darwin" ]]; then
  extra=(-extra-arg="-isysroot$(xcrun --show-sdk-path)")
fi

run-clang-tidy.py -p "$build_dir" -quiet "${extra[@]}" "$PWD/(src|tests)/"
