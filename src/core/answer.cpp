#include "answer.hpp"

#include <cstdint>
#include <optional>

std::optional<Rational> parse_answer(std::string_view input) {
    if (input.length() > 5 || input.length() == 0) {
        return std::nullopt;
    }

    std::int64_t value = 0;
    for (auto c : input) {
        if (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
        } else {
            return std::nullopt;
        }
    }

    return Rational(value);
}
