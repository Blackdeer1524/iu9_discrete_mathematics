#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <stack>
#include <utility>
#include <vector>

// https://www.youtube.com/watch?v=wUgWX0nc4NY
using AdjListT = std::vector<std::vector<uint64_t>>;

class TarjanTraverser {
 public:
    static auto build(const AdjListT &graph)
        -> std::pair<std::vector<uint64_t>, uint64_t> {
        auto traverser = TarjanTraverser(graph);
        return {traverser.low_, traverser.current_component_i};
    }

 private:
    AdjListT              graph_;
    std::vector<bool>     visited_;
    std::vector<bool>     on_stack_;
    std::stack<uint64_t>  stack_;
    std::vector<uint64_t> low_;
    std::vector<uint64_t> ids_;
    uint64_t              timestamp_{0};
    uint64_t              current_component_i{0};

    auto                  dfs(uint64_t at) -> void {
        stack_.push(at);
        on_stack_[at] = true;
        visited_[at]  = true;
        low_[at] = ids_[at] = timestamp_++;
        for (const auto child : graph_[at]) {
            if (!visited_[child]) {
                dfs(child);
            }
            if (on_stack_[child]) {
                low_[at] = std::min(low_[at], low_[child]);
            }
        }
        if (low_[at] == ids_[at]) {
            while (!stack_.empty()) {
                const auto vertex = stack_.top();
                stack_.pop();
                on_stack_[vertex] = false;
                low_[vertex]      = current_component_i;
                if (vertex == at) {
                    break;
                }
            }
            ++current_component_i;
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
          on_stack_(graph_.size()), low_(graph_.size()), ids_(graph_.size()) {
        traverse();
    }
};

auto get_condensation_info(const AdjListT              &graph,
                           const std::vector<uint64_t> &vertex2component,
                           uint64_t                     components_count)
    -> std::pair<std::vector<bool>, std::vector<uint64_t>> {
    std::vector<bool>     is_not_base(components_count);
    std::vector<uint64_t> min_vertex_index_per_condensed_component(
        components_count, std::numeric_limits<uint64_t>::max());
    for (uint64_t parent = 0; parent < graph.size(); ++parent) {
        const auto parent_component = vertex2component.at(parent);
        min_vertex_index_per_condensed_component.at(parent_component) =
            std::min(
                min_vertex_index_per_condensed_component.at(parent_component),
                parent);
        for (const auto child : graph.at(parent)) {
            const auto child_component = vertex2component.at(child);
            if (parent_component != child_component) {
                is_not_base.at(child_component) = true;
            }
        }
    }
    return {is_not_base, min_vertex_index_per_condensed_component};
}

auto eval_result(const AdjListT &graph) -> std::vector<uint64_t> {

    const auto [vertex2component, N] = TarjanTraverser::build(graph);
    const auto [is_not_base, min_vertex_per_component] =
        get_condensation_info(graph, vertex2component, N);

    std::vector<uint64_t> res;
    for (uint64_t i = 0; i < is_not_base.size(); ++i) {
        if (is_not_base[i]) {
            continue;
        }
        res.push_back(min_vertex_per_component.at(i));
    }
    std::sort(res.begin(), res.end());
    return res;
}

// #include <gtest/gtest.h>

// TEST(TarjanTraverser, SingleStronglyConnected) {
//     AdjListT graph{
//         {2, 3},
//         {0},
//         {3},
//         {1}
//     };
//     std::vector<uint64_t> expected = {0, 0, 0, 0};
//     const auto [actual, N]         = TarjanTraverser::build(graph);
//     EXPECT_EQ(1, N);
//     ASSERT_EQ(expected, actual);
// }
//
// TEST(TarjanTraverser, TwoStronglyConnected) {
//     AdjListT graph{
//         {2, 3},
//         {0},
//         {3},
//         {1, 4, 5},
//         {6},
//         {4},
//         {5}
//     };
//     std::vector<uint64_t> expected = {1, 1, 1, 1, 0, 0, 0};
//     const auto [actual, N]         = TarjanTraverser::build(graph);
//     EXPECT_EQ(2, N);
//     ASSERT_EQ(expected, actual);
// }
//
// TEST(get_condensation_info, SingleStronglyConnected) {
//     AdjListT graph{
//         {2, 3},
//         {0},
//         {3},
//         {1}
//     };
//
//     const auto [vertex2component, N] = TarjanTraverser::build(graph);
//     EXPECT_EQ(1, N);
//     const auto [is_not_base, min_vertex_per_component] =
//         get_condensation_info(graph, vertex2component, N);
//     EXPECT_EQ(min_vertex_per_component[0], 0);
//     std::vector<bool> expected{false};
//     ASSERT_EQ(expected, is_not_base);
// }
//
// TEST(get_condensation_info, TwoStronglyConnected) {
//     AdjListT graph{
//         {2, 3},
//         {0},
//         {3},
//         {1, 4, 5},
//         {6},
//         {4},
//         {5}
//     };
//
//     const auto [vertex2component, N] = TarjanTraverser::build(graph);
//     EXPECT_EQ(2, N);
//     const auto [is_not_base, min_vertex_per_component] =
//         get_condensation_info(graph, vertex2component, N);
//
//     std::vector<uint64_t> expected_min_vertecies{4, 0};
//     EXPECT_EQ(expected_min_vertecies, min_vertex_per_component);
//
//     std::vector<bool> expected_not_bases{true, false};
//     ASSERT_EQ(expected_not_bases, is_not_base);
// }
//
// TEST(eval_results, TwoStronglyConnected) {
//     AdjListT graph{
//         {2, 3},
//         {0},
//         {3},
//         {1, 4, 5},
//         {6},
//         {4},
//         {5}
//     };
//     auto                  res = eval_result(graph);
//     std::vector<uint64_t> expected{0};
//     ASSERT_EQ(expected, res);
// }

auto main() -> int {
    uint64_t n;
    uint64_t m;
    std::cin >> n >> m;

    AdjListT graph(n);
    for (uint64_t i = 0; i < m; ++i) {
        uint64_t from;
        uint64_t to;
        std::cin >> from >> to;

        graph.at(from).push_back(to);
    }

    auto res = eval_result(graph);
    for (const auto item : res) {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
}
