# zeta (working name)

A terminal mental-math trainer: Zetamac-style arithmetic plus percentages,
fractions, expected value and probability, with precise timing and local stats.

> Early development. Install instructions and a demo will land with the first release.

## Building from source

Requirements: a C++20 compiler (Clang 15+ / GCC 12+), CMake ≥ 3.24, Ninja,
and SQLite 3 (bundled with macOS; `libsqlite3-dev` on Debian/Ubuntu).
FTXUI, CLI11 and Catch2 are downloaded automatically at configure time.

```bash
cmake --preset dev            # configure (Debug) into build/dev
cmake --build --preset dev    # build
ctest --preset dev            # run tests
./build/dev/zeta --help
```

Other presets: `release`, `asan` (AddressSanitizer + UBSan), and `ci` /
`ci-asan` (the same with `-Werror`, as run in GitHub Actions).

## Layout

| Path | Contents |
|---|---|
| `src/core/` | Rationals, answer parsing, problem generators, wording |
| `src/session/` | Drill session state, scoring, timing |
| `src/store/` | SQLite storage and stats queries |
| `src/tui/` | FTXUI interface |
| `src/cli/` | Command-line entry point |
| `tests/` | Catch2 tests (every `.cpp` here is built into `zeta_tests`) |

Each `src/<area>/` directory becomes a library; new `.cpp` files are picked up
without editing CMake.
