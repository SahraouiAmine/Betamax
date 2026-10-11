#pragma once
#include <cstdint>

class Rational {
    public:
        Rational(std::int64_t num, std::int64_t den = 1);
        [[nodiscard]] std::int64_t num() const;
        [[nodiscard]] std::int64_t denom() const;

    private:
        std::int64_t num_;
        std::int64_t den_;

};
