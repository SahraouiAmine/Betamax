#pragma once
#include <cstdint>

// A Rational is always stored in irreducible form with a positive denominator.
// Constructor throws "std::invalid_argument" if the denominator is 0.
class Rational {
    public:
        Rational(std::int64_t num, std::int64_t den = 1);
        [[nodiscard]] std::int64_t num() const;
        [[nodiscard]] std::int64_t denom() const;

    private:
        std::int64_t num_;
        std::int64_t den_;

};
