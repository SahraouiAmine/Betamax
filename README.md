# Betamax

Zetamac-style arithmetic plus percentages,
fractions, expected value and probability, in the CLI with local stats tracking.

## Building from source

Requirements: a C++20 compiler (Clang 15+ / GCC 12+), CMake ≥ 3.25, Ninja,
and SQLite 3 (`libsqlite3-dev` on Debian/Ubuntu).
FTXUI, CLI11 and Catch2 downloaded automatically at configure time.

```bash
cmake --workflow --preset dev    # configure, build and test (Debug) in build/dev
cmake --workflow --preset asan   # the same with AddressSanitizer + UBSan
./build/dev/betamax --help
```

Each workflow runs these steps, which also work on their own:

```bash
cmake --preset dev            # configure
cmake --build --preset dev    # build
ctest --preset dev            # run tests
```

Other presets: `release`, and `ci` / `ci-asan` (with `-Werror`, as run in
GitHub Actions).

## Layout

| Path | Contents |
|---|---|
| `src/core/` | Rationals, answer parsing, problem generators, wording |
| `src/session/` | Drill session state, scoring, timing |
| `src/store/` | SQLite storage and stats queries |
| `src/tui/` | FTXUI interface |
| `src/cli/` | Command-line entry point |
| `tests/` | Catch2 tests (every `.cpp` here is built into `betamax_tests`) |

Each `src/<area>/` directory becomes a library; new `.cpp` files are picked up
without editing CMake.
