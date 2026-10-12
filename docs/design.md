# Betamax visual design

Scope: v0.1 (start, drill and score screens). The stats screen is sketched at
the end and gets designed properly when its milestone starts.

## Principles

1. **The problem and the answer are the only things that matter.** They sit in
   the middle of the screen, larger by emphasis (bold), with nothing competing.
2. **Calm.** Default terminal colors for everything except state changes. One
   accent color. No animation except the countdown and a brief correct flash.
3. **Peripheral information stays peripheral.** Timer and score live in a thin
   status line, dimmed until they matter (last 10 seconds).
4. **Works everywhere.** 80×24, tmux, light and dark themes, and `NO_COLOR`.

## Layout constraints

- Designed for **80×24**, works down to **40×10**. Below that, show only
  `Terminal too small (need 40×10)` centred.
- Content is a single centred column, at most **48 columns** wide, so it reads
  the same on wide terminals.
- Uses the alternate screen; the terminal is restored on exit, including Ctrl-C.
- Box-drawing characters from FTXUI's light border set only (`┌─┐`, `borderLight`;
  plain `border` is rounded in FTXUI 7). They render
  in every modern terminal and in tmux; no rounded corners, no emoji.

## Screens

### Start

Shown before the timer starts, so the session begins when the player is ready.

```
┌──────────────────────────────────────────────┐
│                                              │
│                   betamax                    │
│                                              │
│          120 seconds · arithmetic            │
│                                              │
│            Press Enter to start              │
│                                              │
│                  Esc quits                   │
│                                              │
└──────────────────────────────────────────────┘
```

- `betamax` bold. Session line normal. "Press Enter to start" in the accent
  color. Key hints dim.

### Drill

```
  1:47                                   score 12
┌──────────────────────────────────────────────┐
│                                              │
│                                              │
│                   12 × 7                     │
│                                              │
│                 ▸ 84▏                        │
│                                              │
│                                              │
└──────────────────────────────────────────────┘
                       Esc quit
```

- **Status line** above the box: remaining time on the left (`m:ss`), score on
  the right. Both dim.
- **Problem**: bold, centred, with a blank line above and below.
- **Answer field**: `▸ ` prompt in the accent color, then the typed text, then
  a cursor. The field is centred as a block, not per character, so it doesn't
  jump while typing.
- **Key hints** below the box, dim.

### Score

```
┌──────────────────────────────────────────────┐
│                                              │
│                   Time's up                  │
│                                              │
│                  score  31                   │
│                                              │
│              31 correct · 2 wrong            │
│                                              │
│       Enter plays again · Esc quits          │
│                                              │
└──────────────────────────────────────────────┘
```

- The score number is bold and in the accent color; everything else normal or
  dim. No judgement words ("great!"): the number speaks.
- The session is already saved when this screen appears.

### Stats (later)

A table per problem type: attempts, accuracy, median time; then best scores.
Designed when the stats milestone starts.

## States

| State | Trigger | Treatment | Duration |
|---|---|---|---|
| Correct | Keystroke completes the right answer | Problem and answer turn **success** color, then the next problem appears | 120 ms, non-blocking: typing continues into the next problem |
| Wrong | Enter on a wrong answer | Answer text turns **error** color; the problem stays; the field clears on the next keystroke | Until next keystroke |
| Time running out | Last 10 seconds | Timer switches from dim to **warning** color and bold | Until the end |
| Time's up | Timer reaches 0 | Input stops; score screen | — |
| Too small | Terminal below 40×10 | Only the size message, centred | Until resized |

The correct flash never delays input: speed is the point of the game.

There is **no skip** in arithmetic drills: the player stays on a problem until
they solve it, so a hard problem costs time rather than being dodged. Skipping
may return per problem type (expected value, probability), where a player may
not know how to approach a problem.

## Theme

Colors use the terminal's 16-color palette, so they follow the user's terminal
theme and stay readable on light and dark backgrounds.

| Role | Color | No-color fallback |
|---|---|---|
| Text | terminal default | — |
| Secondary (timer, score, hints) | default, dim | dim |
| Emphasis (problem) | default, bold | bold |
| Accent (prompt, start, score number) | cyan | bold |
| Success (correct flash) | green | bold |
| Error (wrong answer) | red | inverted |
| Warning (last 10 s) | yellow, bold | bold |
| Border | bright black (dark gray), border only | default |

**No-color mode** is used when the `NO_COLOR` environment variable is set to
any non-empty value (see no-color.org). It keeps every state distinguishable
with bold, dim and inverted text only.

## Spacing

- One blank line between every group (status, problem, answer, hints).
- Two columns of padding inside the box, at minimum.
- Vertical centring inside the box; extra height becomes equal space above
  and below.

The constants live in `src/tui/theme.hpp`.
