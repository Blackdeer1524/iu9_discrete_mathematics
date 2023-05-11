#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

class Vertex {
 public:
    static auto build() -> std::shared_ptr<Vertex> {
        auto node     = std::shared_ptr<Vertex>(new Vertex());
        node->parent_ = node;
        return node;
    }

    auto unite(std::shared_ptr<Vertex> other) -> void {
        link(find(), other->find());
    }

    auto find() -> std::shared_ptr<Vertex> {
        if (this != parent_.get()) {
            parent_ = parent_->find();
        }
        return parent_;
    }

 private:
    Vertex() = default;

    uint64_t                rank_{};
    std::shared_ptr<Vertex> parent_;

    static auto link(std::shared_ptr<Vertex> x, std::shared_ptr<Vertex> y)
        -> void {
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
    Edge(double                  weight,
         std::shared_ptr<Vertex> start,
         std::shared_ptr<Vertex> end)
        : weight_(weight), start_(std::move(start)), end_(std::move(end)) {
    }

    double                  weight_;
    std::shared_ptr<Vertex> start_;
    std::shared_ptr<Vertex> end_;
};

using Coords = std::pair<int, int>;

auto main() -> int {
    uint64_t n;
    std::cin >> n;

    auto points    = std::vector<Coords>();
    auto vertecies = std::vector<std::shared_ptr<Vertex>>();
    for (uint64_t i = 0; i < n; ++i) {
        Coords placeholder;
        std::cin >> placeholder.first >> placeholder.second;
        points.push_back(placeholder);
        vertecies.push_back(Vertex::build());
    }

    std::vector<Edge> edges;
    for (uint64_t i = 0; i < n; ++i) {
        auto first        = points[i];
        auto first_vertex = vertecies[i];
        for (uint64_t j = i + 1; j < n; ++j) {
            auto second        = points[j];
            auto second_vertex = vertecies[j];
            auto weight =
                std::sqrt(std::pow((first.first - second.first), 2) +
                          std::pow((first.second - second.second), 2));
            edges.emplace_back(weight, first_vertex, second_vertex);
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
