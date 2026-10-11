#!/usr/bin/env bash
# Format all C++ sources in place. With --check, change nothing and fail if
# any file is not formatted (this is what CI runs).
set -euo pipefail
cd "$(dirname "$0")/.."

mode=(-i)
if [[ "${1:-}" == "--check" ]]; then
  mode=(--dry-run --Werror)
fi

git ls-files '*.cpp' '*.hpp' | xargs clang-format "${mode[@]}"
