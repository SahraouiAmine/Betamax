#pragma once

#include "core/generators/arithmetic.hpp"

#include <format>
#include <string>

struct TemplateWording {
    [[nodiscard]] std::string prompt(ArithmeticParams params) const {
        switch (params.op) {
        case Op::Add:
            return std::format("{} + {}", params.lhs, params.rhs);

        case Op::Sub:
            return std::format("{} − {}", params.lhs, params.rhs);

        case Op::Mul:
            return std::format("{} × {}", params.lhs, params.rhs);

        case Op::Div:
            return std::format("{} ÷ {}", params.lhs, params.rhs);
        }

        throw std::logic_error("unknown Op");
    }
};
