// Tests for issue #21: Zetamac-style arithmetic problems.
//
// Subtraction and division are addition and multiplication in reverse, so every
// answer is a whole number:  (a + b) − a = b   and   (a × b) ÷ a = b.

#include "core/generators/arithmetic.hpp"
#include "core/problem.hpp"
#include "core/rational.hpp"
#include "core/wording.hpp"

#include <catch2/catch_test_macros.hpp>

#include <random>
#include <set>
#include <vector>

namespace {

constexpr int kDraws = 10'000;

bool in(Range r, std::int64_t x) {
    return r.min <= x && x <= r.max;
}

// Draws kDraws problems of a single operation from a fixed seed.
std::vector<ArithmeticParams> draw(Op op, ArithmeticConfig config = {}) {
    config.ops = {op};
    std::mt19937_64 rng(42);
    std::vector<ArithmeticParams> params;
    params.reserve(kDraws);
    for (int i = 0; i < kDraws; ++i) {
        params.push_back(generate_arithmetic(config, rng));
    }
    return params;
}

} // namespace

TEST_CASE("Answers are exact", "[arithmetic]") {
    REQUIRE(answer_of({Op::Add, 12, 7}) == Rational(19));
    REQUIRE(answer_of({Op::Sub, 19, 7}) == Rational(12));
    REQUIRE(answer_of({Op::Mul, 12, 7}) == Rational(84));
    REQUIRE(answer_of({Op::Div, 84, 12}) == Rational(7));
}

TEST_CASE("Addition draws both terms from their ranges", "[arithmetic]") {
    const ArithmeticConfig config;
    for (const auto& s : draw(Op::Add)) {
        REQUIRE(s.op == Op::Add);
        REQUIRE(in(config.add_left, s.lhs));
        REQUIRE(in(config.add_right, s.rhs));
    }
}

TEST_CASE("Subtraction is addition in reverse", "[arithmetic]") {
    const ArithmeticConfig config;
    for (const auto& s : draw(Op::Sub)) {
        // lhs is a sum a + b; rhs is a; the answer is b.
        REQUIRE(s.op == Op::Sub);
        REQUIRE(in(config.add_left, s.rhs));
        REQUIRE(in(config.add_right, s.lhs - s.rhs));
    }
}

TEST_CASE("Multiplication draws both factors from their ranges", "[arithmetic]") {
    const ArithmeticConfig config;
    for (const auto& s : draw(Op::Mul)) {
        REQUIRE(s.op == Op::Mul);
        REQUIRE(in(config.mul_left, s.lhs));
        REQUIRE(in(config.mul_right, s.rhs));
    }
}

TEST_CASE("Division is multiplication in reverse, so it is always exact", "[arithmetic]") {
    const ArithmeticConfig config;
    for (const auto& s : draw(Op::Div)) {
        // lhs is a product a × b; rhs is a; the answer is b.
        REQUIRE(s.op == Op::Div);
        REQUIRE(in(config.mul_left, s.rhs));
        REQUIRE(s.lhs % s.rhs == 0);
        REQUIRE(in(config.mul_right, s.lhs / s.rhs));
    }
}

TEST_CASE("Every answer is a whole number a player can type", "[arithmetic]") {
    for (Op op : {Op::Add, Op::Sub, Op::Mul, Op::Div}) {
        for (const auto& s : draw(op)) {
            const Rational answer = answer_of(s);
            REQUIRE(answer.denom() == 1);
            REQUIRE(answer.num() >= 0);
            REQUIRE(answer.num() <= 99'999); // decision 2: at most 5 digits
        }
    }
}

TEST_CASE("Custom ranges are respected", "[arithmetic]") {
    const ArithmeticConfig config{.add_left = {.min = 1, .max = 9},
                                  .add_right = {.min = 1, .max = 9}};
    for (const auto& s : draw(Op::Add, config)) {
        REQUIRE(in({1, 9}, s.lhs));
        REQUIRE(in({1, 9}, s.rhs));
    }
}

TEST_CASE("Ranges are inclusive at both ends", "[arithmetic]") {
    // With 10,000 draws over 99 values, missing an endpoint means an
    // off-by-one, not bad luck.
    std::set<std::int64_t> seen;
    for (const auto& s : draw(Op::Add)) {
        seen.insert(s.lhs);
    }
    REQUIRE(seen.contains(2));
    REQUIRE(seen.contains(100));
}

TEST_CASE("The default config mixes all four operations", "[arithmetic]") {
    std::mt19937_64 rng(42);
    std::set<Op> seen;
    for (int i = 0; i < 1'000; ++i) {
        seen.insert(generate_arithmetic({}, rng).op);
    }
    REQUIRE(seen.size() == 4);
}

TEST_CASE("The same seed gives the same problems", "[arithmetic]") {
    // Reproducible on the same platform. The standard library's distributions
    // may differ between libc++ (macOS) and libstdc++ (Linux).
    std::mt19937_64 a(7);
    std::mt19937_64 b(7);
    for (int i = 0; i < 100; ++i) {
        REQUIRE(generate_arithmetic({}, a) == generate_arithmetic({}, b));
    }
}

TEST_CASE("Prompts use the usual symbols", "[wording]") {
    const TemplateWording wording;
    REQUIRE(wording.prompt({Op::Add, 12, 7}) == "12 + 7");
    REQUIRE(wording.prompt({Op::Sub, 19, 7}) == "19 − 7"); // U+2212 minus sign
    REQUIRE(wording.prompt({Op::Mul, 12, 7}) == "12 × 7");
    REQUIRE(wording.prompt({Op::Div, 84, 12}) == "84 ÷ 12");
}
