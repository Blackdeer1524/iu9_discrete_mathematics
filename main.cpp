#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

class Matrix {
 public:
    explicit Matrix(uint64_t N) : N_(N), buffer_(N * N) {
    }

    auto at(uint64_t row, uint64_t col) -> uint64_t & {
        auto &item = buffer_.at(N_ * row + col);
        return item;
    }

 private:
    uint64_t              N_;
    std::vector<uint64_t> buffer_;
};

auto main() -> int {
    uint64_t n;
    std::cin >> n;

    auto field = Matrix(n);
    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < n; ++j) {
            uint64_t value;
            std::cin >> value;
            field.at(i, j) = value;
        }
    }

    auto dynamic_prog_field     = Matrix(n);
    dynamic_prog_field.at(0, 0) = field.at(0, 0);
    for (uint64_t i = 1; i < n; ++i) {
        dynamic_prog_field.at(0, i) = dynamic_prog_field.at(0, i - 1) + field.at(0, i);
        dynamic_prog_field.at(i, 0) = dynamic_prog_field.at(i - 1, 0) + field.at(i, 0);
    }

    for (uint64_t i = 1; i < n; ++i) {
        for (uint64_t j = 1; j < n; ++j) {
            auto min_route_cost = std::min(dynamic_prog_field.at(i, j - 1),
                                           dynamic_prog_field.at(i - 1, j));
            dynamic_prog_field.at(i, j) = field.at(i, j) + min_route_cost;
        }
    }
    std::cout << dynamic_prog_field.at(n - 1, n - 1) << std::endl;
}
