#include <algorithm>
#include <array>
#include <assert.h>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

// Deducing `this` prior to C++23
// https://www.youtube.com/watch?v=5EGw4_NKZlY&t=213s

template <typename T = uint64_t>
    requires std::integral<T> || std::floating_point<T>
class Matrix {
 public:
    explicit Matrix(uint64_t N, T init_value = {})
        : N_(N), buffer_(new T[N * N]) {
        std::fill_n(buffer_.get(), N * N, init_value);
    }

    auto at(uint64_t row, uint64_t col) -> T & {
        return at(*this, row, col);
    }

    [[nodiscard]] auto at(uint64_t row, uint64_t col) const -> const T & {
        return at(*this, row, col);
    }

    [[nodiscard]] auto size() const -> uint64_t {
        return N_;
    }

 private:
    template <typename This>
    static auto at(This &obj, uint64_t row, uint64_t col) -> auto & {
        assert(row < obj.N_);
        assert(col < obj.N_);

        auto &item = obj.buffer_.get()[obj.N_ * row + col];
        return item;
    }

    uint64_t           N_;
    std::unique_ptr<T> buffer_;
};

auto a_star(const Matrix<uint64_t> &map) {
    struct Item {
        Item(uint64_t                      distance,
             std::pair<uint64_t, uint64_t> vertex,
             uint64_t                      field_size)
            : distance_(distance), vertex_(std::move(vertex)),
              field_size_(field_size) {
        }

        auto operator<(const Item &other) const -> bool {
            return distance_ + heuristic() >
                   other.distance_ + other.heuristic();
        }

        [[nodiscard]] auto distance() const -> uint64_t {
            return distance_;
        }

        [[nodiscard]] auto vertex() const -> std::pair<uint64_t, uint64_t> {
            return vertex_;
        }

     private:
        [[nodiscard]] auto heuristic() const -> uint64_t {
            return field_size_ - vertex_.first + field_size_ - vertex_.second;
        }

        std::pair<uint64_t, uint64_t> vertex_;
        uint64_t                      distance_;
        uint64_t                      field_size_;
    };

    auto visited = Matrix<bool>(map.size());
    auto min_distances =
        Matrix<uint64_t>(map.size(), std::numeric_limits<uint64_t>::max());
    min_distances.at(0, 0) = map.at(0, 0);

    auto vertex_queue      = std::priority_queue<Item>();
    vertex_queue.push({
        min_distances.at(0, 0), {0, 0},
          map.size()
    });

    while (!vertex_queue.empty()) {
        const auto item       = vertex_queue.top();
        const auto [row, col] = item.vertex();
        const auto distance   = min_distances.at(row, col);
        vertex_queue.pop();

        constexpr std::array<std::pair<int, int>, 4> moves{
            std::pair<int, int>{-1,  0},
            std::pair<int, int>{ 1,  0},
            std::pair<int, int>{ 0, -1},
            std::pair<int, int>{ 0,  1}
        };

        for (const auto [row_move, col_move] : moves) {
            if (row_move == -1 && row == 0 || col_move == -1 && col == 0 ||
                row_move == 1 && row == map.size() - 1 ||
                col_move == 1 && col == map.size() - 1) {
                continue;
            }
            const auto neighbour_row = row + row_move;
            const auto neighbour_col = col + col_move;
            const auto distance_to_neighbour_from_current =
                distance + map.at(neighbour_row, neighbour_col);
            const auto min_distance_to_neighbour =
                min_distances.at(neighbour_row, neighbour_col);

            if (distance_to_neighbour_from_current <
                min_distance_to_neighbour) {

                min_distances.at(neighbour_row, neighbour_col) =
                    distance_to_neighbour_from_current;

                vertex_queue.push({
                    distance_to_neighbour_from_current,
                    {neighbour_row, neighbour_col},
                    map.size()
                });
            }
        }
    }
    const auto &res = min_distances.at(map.size() - 1, map.size() - 1);
    return res;
}

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

    const auto res = a_star(field);
    std::cout << res << std::endl;
}
