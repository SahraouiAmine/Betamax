#pragma once
#include <cstdint>

class Rational {
    public:
        Rational(int64_t, int64_t);
        [[nodiscard]] std::int64_t num() const;
        [[nodiscard]] std::int64_t denom() const;

    private:
        std::int64_t num_;
        std::int64_t den_;

};
