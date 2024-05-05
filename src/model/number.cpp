#include <gmp.h>
#include <iostream>

#include "model/number.hpp"

int test() {
    // 初始化大整数变量
    mpz_t a, b, c;

    // 赋值操作
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);

    // 为大整数赋值
    mpz_set_str(a, "123456789012345678901234567890", 10);
    mpz_set_str(b, "987654321098765432109876543210", 10);

    // 进行加法运算
    mpz_add(c, a, b);

    // 打印结果
    std::cout << "Sum: " << mpz_get_str(NULL, 10, c) << std::endl;

    // 清除大整数变量
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);

    return 0;
}