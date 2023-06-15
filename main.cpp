#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>

// https://www.youtube.com/watch?v=wUgWX0nc4NY
using AdjListT = std::vector<std::vector<uint64_t>>;
enum class NodeColor {
    BLACK,
    BLUE,  // Вершина лежит в подграфе с циклом
    RED    // Часть критического пути
};

class TarjanTraverser {
 public:
    static auto build(const AdjListT &graph) -> std::vector<NodeColor> {
        auto traverser = TarjanTraverser(graph);
        return traverser.colors_;
    }

 private:
    AdjListT               graph_;
    std::vector<bool>      visited_;
    std::vector<bool>      on_stack_;
    std::stack<uint64_t>   stack_;
    std::vector<uint64_t>  low_;
    std::vector<uint64_t>  ids_;
    std::vector<NodeColor> colors_;
    uint64_t               timestamp_{0};

    auto                   dfs(uint64_t start) -> void {
        stack_.push(start);
        on_stack_[start] = true;
        visited_[start]  = true;
        low_[start] = ids_[start] = timestamp_++;
        for (const auto child : graph_[start]) {
            if (!visited_[child]) {
                dfs(child);
            }
            if (on_stack_[child]) {
                low_[start] = std::min(low_[start], low_[child]);
            }
        }

        // нашли компоненту сильной связности
        if (low_[start] == ids_[start]) {
            // Проверка на петлю
            const auto &children = graph_.at(start);
            if (std::any_of(
                    children.begin(),
                    children.end(),
                    [start](const uint64_t child) { return child == start; })) {
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
        colors_.at(parent) = NodeColor::BLUE;
        for (const auto child : graph_.at(parent)) {
            color_subgraph_blue(child);
        }
    }

    auto traverse() -> void {
        for (uint64_t i = 0; i < graph_.size(); ++i) {
            if (!visited_[i]) {
                dfs(i);
            }
        }
    }

    explicit TarjanTraverser(AdjListT graph)
        : graph_(std::move(graph)), visited_(graph_.size()),
          on_stack_(graph_.size()), low_(graph_.size()), ids_(graph_.size()),
          colors_(graph_.size(), NodeColor::BLACK) {
        traverse();
    }
};

#include <gtest/gtest.h>

TEST(TarjanTraverser, DAG) {
    AdjListT graph{
        {1, 2},
        {2},
        {3},
        {}
    };
    const auto             actual = TarjanTraverser::build(graph);
    std::vector<NodeColor> expected{
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
    };
    ASSERT_EQ(expected, actual);
}

TEST(TarjanTraverser, Cycle) {
    AdjListT graph{
        {1},
        {0},
    };
    const auto             actual = TarjanTraverser::build(graph);
    std::vector<NodeColor> expected{
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    ASSERT_EQ(expected, actual);
}

TEST(TarjanTraverser, SelfLoop) {
    AdjListT graph{
        {0},
    };
    const auto             actual = TarjanTraverser::build(graph);
    std::vector<NodeColor> expected{
        NodeColor::BLUE,
    };
    ASSERT_EQ(expected, actual);
}

TEST(TarjanTraverser, ColoringFurther) {
    AdjListT graph{
        {1},
        {0, 2},
        {3},
        {}
    };
    const auto             actual = TarjanTraverser::build(graph);
    std::vector<NodeColor> expected{
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    ASSERT_EQ(expected, actual);
}

// auto main() -> int {
// }
