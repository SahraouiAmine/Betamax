#include "rational.hpp"
#include <numeric>
#include <stdexcept>

Rational::Rational(std::int64_t num, std::int64_t den) : num_(num), den_(den) {

    // guard against 0 denominator
    if (den == 0) {
        throw std::invalid_argument("Division by 0.");
    }

    // keep in lowest terms
    auto g = std::gcd(num_, den_);
    num_ = num_ / g;
    den_ = den_ / g;

    if (den_ < 0) {
        den_ = -den_;
        num_ = -num_;
    }
}

std::int64_t Rational::num() const {
    return num_;
}

std::int64_t Rational::denom() const {
    return den_;
}
