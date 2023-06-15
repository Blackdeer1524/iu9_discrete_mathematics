#include <algorithm>
#include <bits/fs_fwd.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <queue>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

// https://www.youtube.com/watch?v=wUgWX0nc4NY

struct EdgeInfo {
    uint64_t vertex_name;
    uint64_t weight;
};

using WeightedAdjListT = std::vector<std::vector<EdgeInfo>>;
using AdjListT         = std::vector<std::vector<uint64_t>>;

enum class NodeColor {
    BLACK,
    BLUE,  // Вершина лежит в подграфе с циклом
    RED    // Часть критического пути
};

class TarjanTraverser {
 public:
    static auto build(const WeightedAdjListT &graph)
        -> std::tuple<std::vector<NodeColor>, std::vector<bool>> {
        auto traverser = TarjanTraverser(graph);
        return {traverser.colors_, traverser.is_not_base_};
    }

 private:
    WeightedAdjListT       graph_;
    std::vector<bool>      visited_;
    std::vector<bool>      on_stack_;
    std::stack<uint64_t>   stack_;
    std::vector<uint64_t>  low_;
    std::vector<uint64_t>  ids_;
    std::vector<bool>      is_not_base_;
    std::vector<NodeColor> colors_;
    uint64_t               timestamp_{0};

    auto                   dfs(uint64_t start) -> void {
        stack_.push(start);
        on_stack_[start] = true;
        visited_[start]  = true;
        low_[start] = ids_[start] = timestamp_++;
        for (const auto child : graph_[start]) {
            if (!visited_[child.vertex_name]) {
                is_not_base_.at(child.vertex_name) = true;
                dfs(child.vertex_name);
            }
            if (on_stack_[child.vertex_name]) {
                low_[start] = std::min(low_[start], low_[child.vertex_name]);
            }
        }

        // нашли компоненту сильной связности
        if (low_[start] == ids_[start]) {
            // Проверка на петлю
            const auto &children = graph_.at(start);
            if (std::any_of(children.begin(),
                            children.end(),
                            [start](const auto child) {
                                return child.vertex_name == start;
                            })) {
                color_subgraph_blue(start);
                return;
            }

            if (const auto top = stack_.top(); top == start) {
                stack_.pop();
                return;
            }

            while (!stack_.empty()) {
                const auto vertex = stack_.top();
                stack_.pop();
                on_stack_[vertex] = false;
                if (vertex == start) {
                    break;
                }
            }
            color_subgraph_blue(start);
        }
    }

    auto color_subgraph_blue(uint64_t parent) -> void {
        if (colors_.at(parent) != NodeColor::BLACK) {
            return;
        }
        colors_.at(parent)      = NodeColor::BLUE;
        visited_.at(parent)     = true;
        is_not_base_.at(parent) = true;
        for (const auto child : graph_.at(parent)) {
            color_subgraph_blue(child.vertex_name);
        }
    }

    auto traverse() -> void {
        for (uint64_t i = 0; i < graph_.size(); ++i) {
            if (!visited_[i]) {
                dfs(i);
            }
        }
    }

    explicit TarjanTraverser(WeightedAdjListT graph)
        : graph_(std::move(graph)), visited_(graph_.size()),
          on_stack_(graph_.size()), low_(graph_.size()), ids_(graph_.size()),
          is_not_base_(graph_.size()),
          colors_(graph_.size(), NodeColor::BLACK) {
        traverse();
    }
};

class AStarTraverser {
 public:
    static auto traverse(const WeightedAdjListT       &graph,
                         const std::vector<NodeColor> &colors)
        -> std::vector<std::optional<uint64_t>> {
        AStarTraverser traverser(graph, colors);
        for (uint64_t i = 0; i < traverser.visited_.size(); ++i) {
            if (!traverser.visited_.at(i)) {
                traverser.traverse(i);
            }
        }
        return traverser.max_distances_;
    }

 private:
    explicit AStarTraverser(const WeightedAdjListT       &graph,
                            const std::vector<NodeColor> &colors)
        : graph_(graph), colors_(colors),
          transposed_trimmed_graph_(graph.size()),
          max_distances_(graph_.size()), visited_(graph_.size()){};

    const WeightedAdjListT              &graph_;
    const std::vector<NodeColor>        &colors_;
    AdjListT                             transposed_trimmed_graph_;
    std::vector<std::optional<uint64_t>> max_distances_;
    std::vector<bool>                    visited_;

    auto                                 traverse(uint64_t start) -> void {
        struct Item {
            Item(uint64_t max_distance, uint64_t vertex)
                : max_distance(max_distance), vertex(vertex) {
            }

            uint64_t max_distance;
            uint64_t vertex;

            auto     operator<(const Item &other) const -> bool {
                return max_distance < other.max_distance;
            }
        };

        visited_.at(start) = true;
        if (colors_.at(start) == NodeColor::BLUE) {
            return;
        }

        std::priority_queue<Item> pq;
        pq.emplace(0, start);
        while (!pq.empty()) {
            const auto [total_distance, vertex] = pq.top();
            for (const auto [child, edge_weight] : graph_.at(vertex)) {
                const auto distance_from_current_to_child =
                    total_distance + edge_weight;
                if (max_distances_.at(child) < distance_from_current_to_child) {
                    max_distances_.at(child) = distance_from_current_to_child;
                    transposed_trimmed_graph_.at(child).clear();
                    pq.emplace(distance_from_current_to_child, child);
                }
                transposed_trimmed_graph_.at(child).emplace_back(start);
            }
        }
    }
};

#include <gtest/gtest.h>

TEST(TarjanTraverser, DAG) {
    WeightedAdjListT graph{
        {{1, 0}, {2, 0}},
        {{2, 0}},
        {{3, 0}},
        {}
    };
    const auto [actual_colors, actual_is_not_bases] =
        TarjanTraverser::build(graph);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
    };
    std::vector<bool> expected_not_bases{false, true, true, true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_is_not_bases);
}

TEST(TarjanTraverser, Cycle) {
    WeightedAdjListT graph{
        {{1, 0}},
        {{0, 0}},
    };
    const auto [actual_colors, actual_is_not_bases] =
        TarjanTraverser::build(graph);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true, true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_is_not_bases);
}

TEST(TarjanTraverser, SelfLoop) {
    WeightedAdjListT graph{
        {{0, 0}},
    };
    const auto [actual_colors, actual_is_not_bases] =
        TarjanTraverser::build(graph);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_is_not_bases);
}

TEST(TarjanTraverser, ColoringFurther) {
    WeightedAdjListT graph{
        {{1, 0}},
        {{0, 0}, {2, 0}},
        {{3, 0}},
        {}
    };
    const auto [actual_colors, actual_is_not_bases] =
        TarjanTraverser::build(graph);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true, true, true, true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_is_not_bases);
}

// auto main() -> int {
// }
