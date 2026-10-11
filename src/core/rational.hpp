#pragma once
#include <cstdint>
#include <compare>

// A Rational is always stored in irreducible form with a positive denominator.
// Constructor throws "std::invalid_argument" if the denominator is 0.
class Rational {
    public:
        Rational(std::int64_t num, std::int64_t den = 1);
        [[nodiscard]] std::int64_t num() const;
        [[nodiscard]] std::int64_t denom() const;

        friend bool operator==(const Rational& lhs, const Rational& rhs) {
                return (lhs.num_ == rhs.num_) && (lhs.den_ == rhs.den_);
        }

        friend std::strong_ordering operator<=>(const Rational& lhs, const Rational& rhs) {
            // valid because denominators are always positive
            return (lhs.num_ * rhs.den_) <=> (rhs.num_ * lhs.den_);
        }

        friend Rational operator+(const Rational& lhs, const Rational& rhs) {
            return Rational(lhs.num_ * rhs.den_ + lhs.den_ * rhs.num_, lhs.den_ * rhs.den_);
        }

        friend Rational operator*(const Rational& lhs, const Rational& rhs) {
            return Rational(lhs.num_ * rhs.num_, lhs.den_ * rhs.den_);
        }

        friend Rational operator-(const Rational& lhs, const Rational& rhs) {
            return lhs + (Rational(-1) * rhs);
        }

        friend Rational operator/(const Rational& lhs, const Rational& rhs) {
            return lhs * Rational(rhs.den_, rhs.num_);
        }



    private:
        std::int64_t num_;
        std::int64_t den_;

};
