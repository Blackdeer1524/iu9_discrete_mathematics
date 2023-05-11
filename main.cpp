#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
// #include <memory>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

// НЕЛЬЗЯ ИСПОЛЬЗОВАТЬ УМНЫЕ УКАЗАТЕЛИ :(

class Vertex {
 public:
    auto unite(Vertex *other) -> void {
        link(find(), other->find());
    }

    auto find() -> Vertex * {
        if (this != parent_) {
            parent_ = parent_->find();
        }
        return parent_;
    }

    Vertex() = default;

    auto initialize() -> void {
        parent_ = this;
    }

 private:
    uint64_t    rank_{};
    Vertex     *parent_;

    static auto link(Vertex *x, Vertex *y) -> void {
        if (x->rank_ > y->rank_) {
            y->parent_ = x;
            return;
        }
        x->parent_ = y;
        if (x->rank_ == y->rank_) {
            ++y->rank_;
        }
    }
};

struct Edge {
    Edge(double weight, Vertex *start, Vertex *end)
        : weight_(weight), start_(std::move(start)), end_(std::move(end)) {
    }

    double  weight_;
    Vertex *start_;
    Vertex *end_;
};

using Coords = std::pair<int, int>;

auto main() -> int {
    uint64_t n;
    std::cin >> n;

    auto points    = std::vector<Coords>();
    auto vertecies = std::vector<Vertex>();
    for (uint64_t i = 0; i < n; ++i) {
        Coords placeholder;
        std::cin >> placeholder.first >> placeholder.second;
        points.push_back(placeholder);
        vertecies.emplace_back();
    }

    for (auto &vert : vertecies) {
        vert.initialize();
    }

    std::vector<Edge> edges;
    for (uint64_t i = 0; i < n; ++i) {
        auto  first        = points[i];
        auto *first_vertex = &vertecies[i];
        for (uint64_t j = i + 1; j < n; ++j) {
            auto  second        = points[j];
            auto *second_vertex = &vertecies[j];
            auto  weight =
                std::sqrt(std::pow((first.first - second.first), 2) +
                          std::pow((first.second - second.second), 2));
            edges.emplace_back(Edge(weight, first_vertex, second_vertex));
        }
    }

    std::sort(edges.begin(),
              edges.end(),
              [](const Edge &left, const Edge &right) -> int {
                  return left.weight_ < right.weight_;
              });

    double res = 0;
    for (auto &edge : edges) {
        if (edge.start_->find() != edge.end_->find()) {
            edge.start_->unite(edge.end_);
            res += edge.weight_;
        }
    }
    std::cout.precision(2);
    std::cout << std::fixed;
    std::cout << res << std::endl;
}
