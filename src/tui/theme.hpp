#pragma once
// Visual constants for the TUI. The rules behind them are in docs/design.md.

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <chrono>
#include <cstdlib>

namespace theme {

// Layout (terminal cells).
inline constexpr int kMinWidth = 40;
inline constexpr int kMinHeight = 10;
inline constexpr int kContentWidth = 48; // the centred column never grows wider
inline constexpr int kPadding = 2;       // inside the box, left and right

// Timing.
inline constexpr std::chrono::milliseconds kCorrectFlash{120};
inline constexpr std::chrono::seconds kWarningThreshold{10};

// Text.
inline constexpr const char* kPrompt = "▸ ";

// One decorator per role in docs/design.md, so screens never pick colors
// themselves. Apply with `element | style.role`.
struct Style {
    ftxui::Decorator secondary; // timer, score, hints
    ftxui::Decorator emphasis;  // the problem
    ftxui::Decorator accent;    // prompt, start action, final score
    ftxui::Decorator success;   // correct flash
    ftxui::Decorator error;     // wrong answer
    ftxui::Decorator warning;   // last seconds
    ftxui::Decorator frame;     // the box; styles the border only, never its contents
};

inline Style color_style() {
    using ftxui::Color;
    return {
        .secondary = ftxui::dim,
        .emphasis = ftxui::bold,
        .accent = ftxui::color(Color::Cyan),
        .success = ftxui::color(Color::Green),
        .error = ftxui::color(Color::Red),
        .warning = ftxui::color(Color::Yellow) | ftxui::bold,
        .frame = ftxui::borderStyled(ftxui::LIGHT, Color::GrayDark),
    };
}

// For NO_COLOR: every state stays distinguishable without color.
inline Style mono_style() {
    return {
        .secondary = ftxui::dim,
        .emphasis = ftxui::bold,
        .accent = ftxui::bold,
        .success = ftxui::bold,
        .error = ftxui::inverted,
        .warning = ftxui::bold,
        .frame = ftxui::borderLight,
    };
}

// https://no-color.org: any non-empty NO_COLOR value disables color.
inline bool no_color_requested() {
    const char* value = std::getenv("NO_COLOR");
    return value != nullptr && value[0] != '\0';
}

inline Style style() {
    return no_color_requested() ? mono_style() : color_style();
}

} // namespace theme
