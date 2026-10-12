// Tests for issue #20: parsing whole-number answers (the v0.1 subset of #5).
//
// Proposed interface (yours to change): core/answer.hpp declares
//     std::optional<Rational> parse_answer(std::string_view input);
// It returns a Rational, not an integer, so decimals and fractions (#5) can be
// added later without changing callers.

#include "core/answer.hpp"
#include "core/rational.hpp"

#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string_view>

TEST_CASE("Whole numbers parse to their value", "[answer]") {
    REQUIRE(parse_answer("0") == Rational(0));
    REQUIRE(parse_answer("7") == Rational(7));
    REQUIRE(parse_answer("42") == Rational(42));
}

TEST_CASE("Five digits is the maximum", "[answer]") {
    REQUIRE(parse_answer("99999") == Rational(99999));
    REQUIRE(parse_answer("123456") == std::nullopt);
}

TEST_CASE("Leading zeros are allowed and count toward the five digits", "[answer]") {
    REQUIRE(parse_answer("007") == Rational(7));
    REQUIRE(parse_answer("00000") == Rational(0));
    REQUIRE(parse_answer("000007") == std::nullopt);
}

TEST_CASE("Empty and partial input is rejected", "[answer]") {
    REQUIRE(parse_answer("") == std::nullopt);
    REQUIRE(parse_answer("-") == std::nullopt);
}

TEST_CASE("Anything but digits is rejected", "[answer]") {
    REQUIRE(parse_answer("1a") == std::nullopt);
    REQUIRE(parse_answer(" 1") == std::nullopt);
    REQUIRE(parse_answer("1 ") == std::nullopt);
    REQUIRE(parse_answer("+1") == std::nullopt);
}

TEST_CASE("Only ASCII digits count as digits", "[answer]") {
    REQUIRE(parse_answer("²") == std::nullopt);  // superscript two
    REQUIRE(parse_answer("１") == std::nullopt); // fullwidth digit one
    REQUIRE(parse_answer("٣") == std::nullopt);  // Arabic-Indic digit three
}

TEST_CASE("Decimals and fractions are not accepted yet", "[answer]") {
    // v0.1 answers are whole numbers; #5 extends the parser.
    REQUIRE(parse_answer("1.5") == std::nullopt);
    REQUIRE(parse_answer("1/2") == std::nullopt);
}
