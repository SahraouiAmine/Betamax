#pragma once

#include "core/rational.hpp"

#include <random>
#include <vector>

enum class Op : char {
    Add,
    Sub,
    Mul,
    Div,
};

struct Range {
    int min;
    int max;
};

// defaults: (2–100) + (2–100) and (2–12) × (2–100)
struct ArithmeticConfig {
    std::vector<Op> ops = {Op::Add, Op::Sub, Op::Mul, Op::Div};

    Range add_left = {.min = 2, .max = 100};
    Range add_right = {.min = 2, .max = 100};

    Range mul_left = {.min = 2, .max = 12};
    Range mul_right = {.min = 2, .max = 100};
};

struct ArithmeticParams {
    Op op;
    int lhs;
    int rhs;

    bool operator==(const ArithmeticParams& other) const {
        return (op == other.op && lhs == other.lhs && rhs == other.rhs);
    }
};

ArithmeticParams generate_arithmetic(const ArithmeticConfig& config, std::mt19937_64& rng);
Rational answer_of(ArithmeticParams);
