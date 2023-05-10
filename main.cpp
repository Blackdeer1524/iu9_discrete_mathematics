#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

auto get_divisors(uint64_t n) -> std::vector<uint64_t> {
    std::vector<uint64_t> res;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) {
            continue;
        }
        res.push_back(i);
        if (i * i != n) {
            res.push_back(n / i);
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

auto is_prime(uint64_t item) -> bool {
    if (item <= 2) {
        return true;
    }
    if (item % 2 == 0) {
        return false;
    }
    for (uint64_t i = 3; i * i <= item; i += 2) {
        if (item % i == 0) {
            return false;
        }
    }
    return true;
}

auto quotient_is_prime(uint64_t divident, uint64_t divisor) -> bool {
    auto quotient = divident / divisor;
    auto test     = get_divisors(quotient);
    return is_prime(quotient);
}

auto main() -> int {
    uint64_t n;
    std::cin >> n;
    if (n == 1) {
        std::cout << "graph {\n1\n}" << std::endl;
        return 0;
    }
    auto divisors = get_divisors(n);
    std::cout << "graph {" << std::endl;
    for (uint64_t i = 0; i < divisors.size(); ++i) {
        auto divisor = divisors[i];
        std::cout << '\t' << divisor << std::endl;
        for (uint64_t j = 0; j < i; ++j) {
            if (divisor % divisors[j] == 0 &&
                quotient_is_prime(divisor, divisors[j])) {
                std::cout << '\t' << divisor << " -- " << divisors[j]
                          << std::endl;
            }
        }
    }
    std::cout << '}' << std::endl;
}
