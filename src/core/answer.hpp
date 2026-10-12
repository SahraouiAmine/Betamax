#pragma once
#include "rational.hpp"
#include <optional>
#include <string_view>

std::optional<Rational> parse_answer(std::string_view input);
