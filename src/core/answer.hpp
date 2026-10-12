#pragma once
#include "core/rational.hpp"

#include <optional>
#include <string_view>

// accepts 1 to 5 ASCII digits
[[nodiscard]] std::optional<Rational> parse_answer(std::string_view input);
