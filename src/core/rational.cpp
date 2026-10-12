#include "rational.hpp"

#include <numeric>
#include <optional>
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

int64_t ipow(int base, uint exp) {
    int64_t result = 1;
    while (exp > 0) {
        if (exp & 1) {result *= base;}
        base *= base;
        exp >>= 1;
    }
    return result;
}

std::optional<std::string> Rational::to_decimal() const {

    std::optional<std::string> decimal_representation;
    // a quick edge case
    if (den_ == 1) {
        decimal_representation = std::to_string(num_);
        return decimal_representation;
    }

    // an irreducible rational has an ending decimal rep
    // iff the denominator only has 2s and 5s as prime factors
    // so let's check, also saving the number of factors
    auto denominator = den_;
    int number_2s = 0;
    int number_5s = 0;
    while (denominator % 2 == 0) {
        denominator /= 2;
        number_2s += 1;
    }
    // then divide out 5s, unti w
    while (denominator > 1) {
        denominator /= 5;
        number_5s += 1;
    }

    if (denominator == 1) {
        // otherwise we can represent as a terminating decimal
        uint k = std::max(number_2s, number_5s); // the number of decimal places
        auto factor = ipow(10, k) / den_;
        auto decimals = factor * num_; // these are the decimals in the reporesentation
        // all we need to do is insert the "."
        std::string decimal_string = std::to_string(decimals);
        if (decimal_string.length() == k) {
            // we need to insert a 0 and the decimal
            decimal_representation = "0." + decimal_string;
        }
        else if (uint l = decimal_string.length(); l > k) {
            // we cout k from the back and insert
            decimal_representation = decimal_string.substr(0, (l - k))
 + "." + decimal_string.substr((l - k), l);
        }
        else {
            decimal_representation = decimal_string;
        }
    }
    else {
        decimal_representation = std::nullopt;
    }

    return decimal_representation;

}
