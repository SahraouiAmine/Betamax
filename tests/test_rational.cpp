#include <catch2/catch_test_macros.hpp>
#include "core/rational.hpp"
#include <stdexcept>

TEST_CASE("Rational is stored in lowest terms 1", "[rational]") {
    Rational r(2, 4);
    REQUIRE(r.num() == 1 );
    REQUIRE(r.denom() == 2);
}

TEST_CASE("Rational is stored in lowest terms 2", "[rational]") {
    Rational r(6, 4);
    REQUIRE(r.num() == 3);
    REQUIRE(r.denom() == 2);
}

TEST_CASE("Denominator is always positive", "[rational]") {
    Rational r(1, -2);
    REQUIRE(r.num() == -1);
    REQUIRE(r.denom() == 2);
}

TEST_CASE("Irreducible form of 0/N is 0/1", "[rational]") {
    Rational r(0, 5);
    REQUIRE(r.num() == 0);
    REQUIRE(r.denom() == 1);
}

TEST_CASE("Integer input is stored as N/1", "[rational]") {
    Rational r(3);
    REQUIRE(r.num() == 3);
    REQUIRE(r.denom() == 1);
}

TEST_CASE("Exception is thrown on 0 denominator input", "[rational]") {
    REQUIRE_THROWS_AS(Rational(1, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(Rational(0, 0), std::invalid_argument);
}

TEST_CASE("Rationals can be compared for equality", "[rational]") {
    Rational a(1, 2);
    Rational b(2, 4);
    REQUIRE(a == b);
}

TEST_CASE("Rationals can be compared for inequality", "[rational]") {
    Rational a(1, 2);
    Rational b(3, 4);
    REQUIRE(a != b);
}

TEST_CASE("Rationals can be ordered", "[rational]") {
    Rational a(1,2);
    Rational b(3, 4);
    Rational c(2, 4);
    Rational d(1, 3);

    REQUIRE(a < b);
    REQUIRE(a <= c);
    REQUIRE(a >= c);
    REQUIRE(d < a);
    REQUIRE(Rational(-1, 2) < Rational(1, 3));
}

TEST_CASE("Rationals can be added together", "[rational]") {
    REQUIRE(Rational(1,2) + Rational(1,3) == Rational(5,6));
}

TEST_CASE("Rationals can be multiplied together", "[rational]") {
    REQUIRE(Rational(1,2) * Rational(1,3) == Rational(1,6));
    REQUIRE(Rational(1,2) * Rational(1) == Rational(1,2));
    REQUIRE(Rational(1,2) * Rational(0) == Rational(0));
}

TEST_CASE("Rationals can be subtracted from one another","[rational]") {
    REQUIRE(Rational(1,2) - Rational(1,3) == Rational(1,6));
    REQUIRE(Rational(1,2) - Rational(1,2) == Rational(0));
    REQUIRE(Rational(1,2) - Rational(-1,2) == Rational(1));
}

TEST_CASE("Rationals can be divided by other Rationals","[rational]") {
    REQUIRE(Rational(1,2) / Rational(1,3) == Rational(3,2));
    REQUIRE(Rational(1,2) / Rational(1,2) == Rational(1));
    REQUIRE(Rational(1,2) / Rational(-1,2) == Rational(-1));
}
