#include "arithmetic.hpp"

#include "core/rational.hpp"

#include <random>
#include <stdexcept>

ArithmeticParams generate_arithmetic(const ArithmeticConfig& config, std::mt19937_64& rng) {

    // first I have to decide on the operation
    // I do this randomly
    int ops_list_size = static_cast<int>(config.ops.size());
    std::uniform_int_distribution<> distop(0, ops_list_size - 1);
    int random_num = distop(rng);
    Op op = config.ops[random_num];

    int lhs = 0;
    int rhs = 0;

    // now we pick operands in the correct ranges
    switch (op) {
    case Op::Add: {
        std::uniform_int_distribution<> dist_add_sub_left(config.add_left.min, config.add_left.max);
        std::uniform_int_distribution<> dist_add_sub_right(config.add_right.min,
                                                           config.add_right.max);
        lhs = dist_add_sub_left(rng);
        rhs = dist_add_sub_right(rng);
        break;
    }
    case Op::Sub: {
        std::uniform_int_distribution<> dist_add_sub_left(config.add_left.min, config.add_left.max);
        std::uniform_int_distribution<> dist_add_sub_right(config.add_right.min,
                                                           config.add_right.max);
        lhs = dist_add_sub_right(rng);
        rhs = dist_add_sub_left(rng);
        int ans = lhs + rhs; // we wanna give ans - rhs = lhs
        lhs = ans;
        break;
    }
    case Op::Mul: {
        std::uniform_int_distribution<> dist_mul_div_left(config.mul_left.min, config.mul_left.max);
        std::uniform_int_distribution<> dist_mul_div_right(config.mul_right.min,
                                                           config.mul_right.max);
        lhs = dist_mul_div_left(rng);
        rhs = dist_mul_div_right(rng);
        break;
    }
    case Op::Div: {
        std::uniform_int_distribution<> dist_mul_div_left(config.mul_left.min, config.mul_left.max);
        std::uniform_int_distribution<> dist_mul_div_right(config.mul_right.min,
                                                           config.mul_right.max);
        lhs = dist_mul_div_right(rng);
        rhs = dist_mul_div_left(rng);
        int ans = lhs * rhs; // we wanna give them ans / rhs = lhs
        lhs = ans;
        break;
    }
    }

    return {.op = op, .lhs = lhs, .rhs = rhs};
}

Rational answer_of(ArithmeticParams params) {

    switch (params.op) {
    case Op::Add:
        return {params.lhs + params.rhs};

    case Op::Sub:
        return {params.lhs - params.rhs};

    case Op::Mul:
        return {static_cast<std::int64_t>(params.lhs) * params.rhs};

    case Op::Div:
        return {params.lhs / params.rhs};
    }

    throw std::logic_error("unknown Op");
}
