#include <catch2/catch_test_macros.hpp>
#include "core/rational.hpp"

TEST_CASE("Irreducible fractions only, less than unity.", "[rational]") {
    Rational r(2, 4);
    REQUIRE(r.num() == 1 );
    REQUIRE(r.denom() == 2);
}

TEST_CASE("Irreducible fractions only, larger than unity.", "[rational]") {
    Rational r(6, 4);
    REQUIRE(r.num() == 3);
    REQUIRE(r.denom() == 2);
}
