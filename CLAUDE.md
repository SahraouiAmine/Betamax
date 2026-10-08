# Zeta (working name) — Stage 1

A terminal mental-math trainer in C++. Stage 1 delivers a tool my friends can install with Homebrew and use daily: Zetamac-style arithmetic plus higher-level problems (percentages, fractions, expected value, probability), precise timing, local session history, and a stats view.

Stage 2 (later, not in scope here) adds a small fine-tuned language model that rewrites structured problems into varied natural-language wording, running locally inside the binary. Stage 1 must leave clean seams for it (see "Stage 2 seams").

Rename the project before the first public release.

---

## How we work: ownership rules

This project is for learning. Follow these rules strictly, even if asked to "just do it".

| Area | Owner | Claude Code's role |
|---|---|---|
| `src/core/` (rationals, answer parsing, checking, generators) | **Me** | Explain, review, suggest test cases. Do not write the code. |
| `src/store/` (SQLite layer, SQL queries) | **Me** | Same as above. I'm rebuilding my SQL, so let me write every query. |
| `src/session/` (drill session logic, scoring) | **Me** | Same as above. |
| `src/tui/` (everything except `theme.hpp`) | **Me** | **Teacher.** Run the TUI lessons below. Never write files here. |
| `src/tui/theme.hpp`, `docs/design.md` | **Claude Code** | Owns visual design: layout, colors, borders, spacing, states. |
| `tests/` for core, store, session | **Me** | Propose cases in chat; I write them. |
| Build, deps, CI, release, Homebrew tap, README | **Claude Code** | Write and maintain. Explain anything non-obvious. |
| CLI argument wiring (`src/cli/main.cpp` glue) | **Claude Code** | Wire CLI11 to functions I've written. No business logic. |

When I ask for help in an area I own:
- Explain the concept, ask a guiding question, or point to the relevant docs.
- Review my diffs and flag bugs, edge cases, and undefined behaviour.
- Illustrative snippets are allowed in chat only: at most ~15 lines, and not the exact function I'm writing.
- Never edit files in my areas. If you think a change is needed, describe it and let me make it.

If a request would break these rules, say so and offer the teaching version instead.

---

## Tech stack

- C++20, CMake (≥ 3.24), dependencies via `FetchContent`
- TUI: **FTXUI**
- CLI parsing: **CLI11**
- Storage: **SQLite3** (system library on macOS/Linux; C API)
- Tests: **Catch2 v3**, including property-style tests with Catch2 generators
- CI: GitHub Actions, matrix `macos-latest` + `ubuntu-latest`
- Distribution: GitHub Releases + a Homebrew tap

Warnings on (`-Wall -Wextra -Wpedantic`, plus `-Werror` in CI), sanitizers (ASan, UBSan) in a debug CI job.

---

## Directory layout

```
CMakeLists.txt
src/
  cli/main.cpp            # Claude Code: CLI11 wiring only
  core/
    rational.hpp/.cpp     # Me: exact rational type
    answer.hpp/.cpp       # Me: parse user input into a Rational
    problem.hpp           # Me: ProblemSpec, Problem structs
    generators/           # Me: one file per problem type + registry
    wording.hpp/.cpp      # Me: Wording interface + TemplateWording
  session/
    session.hpp/.cpp      # Me: drill session state, scoring, timing records
  store/
    db.hpp/.cpp           # Me: SQLite schema, inserts, stats queries
  tui/
    theme.hpp             # Claude Code: colors, borders, spacing constants
    app.cpp, screens/...  # Me (taught by Claude Code)
scratch/                  # Me: TUI lesson exercises, not shipped
tests/
docs/
  design.md               # Claude Code: visual design spec
  decisions.md            # Me: short log of design decisions and why
```

---

## Open decisions (mine — confirm before the milestone that needs them)

1. **v1 problem types** (needed for M2). Proposed:
   - Arithmetic: + − × ÷, Zetamac-style ranges (configurable)
   - Percentages: "x% of y", "what % of y is x", percentage change
   - Fractions: fraction ↔ decimal ↔ percent conversion
   - Expected value: dice, coins, simple lotteries
   - Probability: simple events with dice, coins, cards, urns
2. **Accepted answer formats** (needed for M1). Proposed: integers, finite decimals, fractions `a/b`, and `x%` where the problem asks for a percent. Non-terminating answers (e.g. 1/6) accept the fraction, or a decimal rounded to 3 places.
3. **Input behaviour** (needed for M3). Proposed, Zetamac-like: a correct answer auto-advances on the keystroke that completes it; Enter submits a wrong answer explicitly (logged as wrong, problem stays); Tab skips (logged as skipped).
4. **Default session**: 120 seconds, mixed types; score = number correct.
5. **Final name.**

---

## Core design (mine to implement)

### Rational
- `int64_t` numerator/denominator, always normalized (gcd, positive denominator).
- Arithmetic operators, comparison, conversion to/from finite decimal strings.
- Overflow must be detected, not silent. Decide how (checked arithmetic or `__int128` intermediates) and record it in `docs/decisions.md`.

### Problem model
```
ProblemSpec  — type + parameters (structured, serializable to JSON)
Problem      — spec + prompt text + exact answer (Rational) + answer format hint
```
Generators produce a `ProblemSpec` and the exact answer. **Wording is separate**: a `Wording` interface turns a spec into prompt text. Stage 1 ships `TemplateWording` (fixed templates). Stage 2 adds `ModelWording`.

### Generators
- Pure functions of `(config, rng)`, so a fixed seed reproduces a session (`--seed`).
- Invariants to test as properties, e.g.: answer is exactly representable; percentage problems have "nice" answers when configured; EV answers equal the brute-force enumeration over outcomes; probabilities lie in [0, 1].
- A registry maps type names to generators, used by `--focus`.

### Timing
- `std::chrono::steady_clock` only.
- `shown_at` = when the problem is first rendered (the TUI tells the session layer); `response_ms` = from `shown_at` to the keystroke that completes the answer.
- Document any known measurement lag (render loop delay) in `docs/decisions.md`.

### Storage schema (I write the SQL)
```
sessions(id, started_at, ended_at, duration_s, focus, seed, score)
attempts(id, session_id, problem_type, spec_json, prompt,
         correct_answer, user_answer, outcome,      -- correct | wrong | skipped
         response_ms, shown_at)
```
- Database in the platform's standard data directory (`~/.local/share/<name>/` on Linux, `~/Library/Application Support/<name>/` on macOS); Claude Code may provide the path helper.
- Schema versioning via `PRAGMA user_version` from day one.
- Stats queries: per-type accuracy, median response time, attempts over time, best scores.

Log everything from the first release. A later adaptive selector will be designed against this data.

---

## CLI surface

```
<name>                         # default 120s mixed session
<name> --focus ev,pct          # restrict to problem types
<name> --duration 60
<name> --seed 42               # reproducible session
<name> stats                   # stats view
<name> --version
```

---

## TUI curriculum (Claude Code teaches, I build)

Goal: by the end, I can design and build a TUI on my own. Each lesson is: concept explanation → small exercise I write in `scratch/` (not shipped) → I apply it to the real app → Claude Code reviews.

**L1 — How terminals work.** Cooked vs raw mode, `termios`, ANSI escape codes, the alternate screen, why programs must restore terminal state on exit. Exercise: a ~30-line raw-mode program by hand, with no library, that reads keys and redraws a line.

**L2 — FTXUI's model.** Elements (immutable render tree) vs Components (state + event handling), the render loop, `ScreenInteractive`. Exercise: render a static mockup of the drill screen from `docs/design.md`.

**L3 — Input.** `CatchEvent`, building a custom answer field (digits, `.`, `/`, `%`, `-`, backspace), auto-advance on correct, Enter and Tab behaviour.

**L4 — Time and threads.** A visible countdown and live timer: redraw cadence, posting events from a timer thread (`PostEvent`), what's thread-safe and what isn't, measuring with `steady_clock` without blocking rendering. This lesson also prepares Stage 2's background pre-generation.

**L5 — Screens and state.** An app state machine (start → drill → summary → stats), passing data between the TUI and the session layer cleanly, keeping logic out of the TUI.

**L6 — Robustness and polish.** Terminal resize, small terminals, `NO_COLOR` support and a no-color fallback, clean exit on Ctrl-C with terminal restoration, testing what can be tested.

Claude Code: pace lessons to my progress, quiz me briefly before moving on, and don't skip L1 even though FTXUI hides it.

---

## Visual design (Claude Code owns)

Write `docs/design.md` before L2 with:
- Screen layouts: start, drill, summary, stats (ASCII mockups)
- Theme: palette (with a no-color fallback), border styles, spacing, emphasis
- States: correct flash, wrong submission, skip, time running out
- Constraints: minimum terminal size, readable at 80×24, nothing that breaks in tmux

Then implement `src/tui/theme.hpp` with the constants. Keep it minimal and calm: the problem and the answer field are the focus.

---

## Milestones (~13 hours total)

| # | Milestone | Owner | Est. |
|---|---|---|---|
| M0 | Repo scaffold: CMake, FetchContent deps, empty targets, CI (build + test, macOS/Linux, sanitizer job) | Claude Code | 1h |
| M1 | `Rational`, answer parser, checker + tests | Me | 2h |
| M2 | Generators for the v1 types, registry, `TemplateWording`, property tests | Me | 3h |
| M3 | Design doc + theme (Claude Code), TUI lessons L1–L4 and the drill screen (me) | Both | 3.5h |
| M4 | SQLite layer, logging, `stats` command | Me | 2h |
| M5 | Session flow: `--focus`, `--duration`, `--seed`, summary screen (L5–L6) | Me (taught) | 1h |
| M6 | Release workflow, Homebrew tap, README with a demo GIF | Claude Code | 1h |

### Definition of done for Stage 1
- A friend can run `brew install <tap>/<name>` and complete a 120-second session.
- All attempts are logged; `stats` shows per-type accuracy and median time.
- CI passes on macOS and Linux, including the sanitizer job.
- `docs/decisions.md` records the main design choices in my own words.

---

## Stage 2 seams (build these in Stage 1, use them later)

- The `Wording` interface is the only way prompt text is produced.
- Generators are pure and seedable, so problems can be produced ahead of time.
- The session consumes problems from a queue-like source rather than calling generators directly, so a background producer can feed it later.
- `spec_json` is stored for every attempt, so model wordings can be evaluated against the same specs.

Do not build any of Stage 2 now.
