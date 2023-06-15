#include <algorithm>
#include <bits/fs_fwd.h>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// https://www.youtube.com/watch?v=wUgWX0nc4NY

struct EdgeInfo {
    uint64_t vertex_name;
    uint64_t weight;
};

using WeightedAdjListT = std::vector<std::set<EdgeInfo>>;
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
        std::tuple<std::vector<NodeColor>, std::vector<bool>> res{
            traverser.colors_, traverser.not_base_};
        return res;
    }

 private:
    WeightedAdjListT       graph_;
    std::vector<bool>      visited_;
    std::vector<bool>      on_stack_;
    std::stack<uint64_t>   stack_;
    std::vector<uint64_t>  low_;
    std::vector<uint64_t>  ids_;
    std::vector<bool>      not_base_;
    std::vector<NodeColor> colors_;
    uint64_t               timestamp_{0};

    auto                   dfs(uint64_t start) -> void {
        stack_.push(start);
        on_stack_[start] = true;
        visited_[start]  = true;
        low_[start] = ids_[start] = timestamp_++;
        for (const auto child : graph_[start]) {
            not_base_.at(child.vertex_name) = true;
            if (!visited_[child.vertex_name]) {
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
        colors_.at(parent)   = NodeColor::BLUE;
        visited_.at(parent)  = true;
        not_base_.at(parent) = true;
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
          not_base_(graph_.size()), colors_(graph_.size(), NodeColor::BLACK) {
        traverse();
    }
};

#include <assert.h>

class DijkstraTraverser {
 public:
    static auto traverse(const WeightedAdjListT       &graph,
                         const std::vector<NodeColor> &colors,
                         const std::vector<bool>      &not_bases)
        -> std::tuple<std::vector<std::optional<uint64_t>>, AdjListT> {
        DijkstraTraverser traverser(graph, colors);
        for (uint64_t i = 0; i < traverser.visited_.size(); ++i) {
            if (/* !traverser.visited_.at(i) &&  */ !not_bases.at(i)) {
                assert(!traverser.visited_.at(i));
                traverser.max_distances_.at(i) = 0;
                traverser.traverse(i);
            }
        }
        return {traverser.max_distances_, traverser.transposed_trimmed_graph_};
    }

 private:
    explicit DijkstraTraverser(const WeightedAdjListT       &graph,
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
            pq.pop();
            for (const auto [child, edge_weight] : graph_.at(vertex)) {
                if (colors_.at(child) == NodeColor::BLUE) {
                    // Такое возможно когда есть внешнее ребро к синему подграфу
                    continue;
                }

                const auto distance_from_current_to_child =
                    total_distance + edge_weight;
                if (max_distances_.at(child) < distance_from_current_to_child) {
                    max_distances_.at(child) = distance_from_current_to_child;
                    transposed_trimmed_graph_.at(child).clear();
                    pq.emplace(distance_from_current_to_child, child);
                    transposed_trimmed_graph_.at(child).emplace_back(start);
                } else if (max_distances_.at(child) ==
                           distance_from_current_to_child) {
                    transposed_trimmed_graph_.at(child).emplace_back(start);
                }
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
    const auto             res              = TarjanTraverser::build(graph);
    const auto             actual_colors    = std::get<0>(res);
    const auto             actual_not_bases = std::get<1>(res);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
    };
    std::vector<bool> expected_not_bases{false, true, true, true};

    EXPECT_EQ(expected_colors.size(), actual_colors.size());
    EXPECT_EQ(expected_not_bases.size(), actual_not_bases.size());

    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_not_bases);
}

TEST(TarjanTraverser, Cycle) {
    WeightedAdjListT graph{
        {{1, 0}},
        {{0, 0}},
    };
    const auto             res              = TarjanTraverser::build(graph);
    const auto             actual_colors    = std::get<0>(res);
    const auto             actual_not_bases = std::get<1>(res);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true, true};

    EXPECT_EQ(expected_colors.size(), actual_colors.size());
    EXPECT_EQ(expected_not_bases.size(), actual_not_bases.size());

    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_not_bases);
}

TEST(TarjanTraverser, SelfLoop) {
    WeightedAdjListT graph{
        {{0, 0}},
    };
    const auto             res              = TarjanTraverser::build(graph);
    const auto             actual_colors    = std::get<0>(res);
    const auto             actual_not_bases = std::get<1>(res);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_not_bases);
}

TEST(TarjanTraverser, ColoringFurther) {
    WeightedAdjListT graph{
        {{1, 0}},
        {{0, 0}, {2, 0}},
        {{3, 0}},
        {}
    };
    const auto             res              = TarjanTraverser::build(graph);
    const auto             actual_colors    = std::get<0>(res);
    const auto             actual_not_bases = std::get<1>(res);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
    };
    std::vector<bool> expected_not_bases{true, true, true, true};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_not_bases);
}

TEST(TarjanTraverser, PartialDAG) {
    WeightedAdjListT graph{
        {{1, 0}},
        {{0, 0}, {2, 0}},
        {{3, 0}},
        {},
        {{3, 0}},
        {{4, 0}},
        {{4, 0}}
    };
    const auto             res              = TarjanTraverser::build(graph);
    const auto             actual_colors    = std::get<0>(res);
    const auto             actual_not_bases = std::get<1>(res);
    std::vector<NodeColor> expected_colors{
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLUE,
        NodeColor::BLACK,
        NodeColor::BLACK,
        NodeColor::BLACK,
    };
    std::vector<bool> expected_not_bases{
        true, true, true, true, true, false, false};
    EXPECT_EQ(expected_colors, actual_colors);
    EXPECT_EQ(expected_not_bases, actual_not_bases);
}

TEST(DijkstraTraverser, DAG) {
    WeightedAdjListT graph{
        {{1, 5}, {2, 7}},
        {{2, 5}},
        {{3, 3}},
        {}
    };
    const auto res              = TarjanTraverser::build(graph);
    const auto actual_colors    = std::get<0>(res);
    const auto actual_not_bases = std::get<1>(res);
    const auto [actual_distances, transposed_graph] =
        DijkstraTraverser::traverse(graph, actual_colors, actual_not_bases);

    std::vector<std::optional<uint64_t>> expected_distances{0, 5, 10, 13};
    ASSERT_EQ(expected_distances, actual_distances);
}

TEST(DijkstraTraverser, Cycle) {
    WeightedAdjListT graph{{{1, 2}}, {{0, 3}}};
    const auto       res              = TarjanTraverser::build(graph);
    const auto       actual_colors    = std::get<0>(res);
    const auto       actual_not_bases = std::get<1>(res);
    const auto [actual_distances, transposed_graph] =
        DijkstraTraverser::traverse(graph, actual_colors, actual_not_bases);

    std::vector<std::optional<uint64_t>> expected_distances{std::nullopt,
                                                            std::nullopt};
    ASSERT_EQ(expected_distances, actual_distances);
}

TEST(DijkstraTraverser, ColoredSegment) {
    WeightedAdjListT graph{
        {{1, 2}},
        {{0, 3}, {2, 10}},
        {{3, 1}},
        {}
    };

    const auto res              = TarjanTraverser::build(graph);
    const auto actual_colors    = std::get<0>(res);
    const auto actual_not_bases = std::get<1>(res);
    const auto [actual_distances, transposed_graph] =
        DijkstraTraverser::traverse(graph, actual_colors, actual_not_bases);

    std::vector<std::optional<uint64_t>> expected_distances{
        std::nullopt, std::nullopt, std::nullopt, std::nullopt};
    ASSERT_EQ(expected_distances, actual_distances);
}

TEST(DijkstraTraverser, PartialDAG) {
    WeightedAdjListT graph{
        {{1, 2}},
        {{0, 3}, {2, 10}},
        {{3, 1}},
        {},
        {{3, 3}},
        {{4, 5}},
        {{4, 6}}
    };
    const auto res              = TarjanTraverser::build(graph);
    const auto actual_colors    = std::get<0>(res);
    const auto actual_not_bases = std::get<1>(res);
    const auto [actual_distances, actual_transposed_graph] =
        DijkstraTraverser::traverse(graph, actual_colors, actual_not_bases);

    std::vector<std::optional<uint64_t>> expected_distances{
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, 6, 0, 0};
    AdjListT expected_transposed_graph{
        {},
        {},
        {},
        {},
        {6},
        {},
        {},
    };

    ASSERT_EQ(expected_distances, actual_distances);
    ASSERT_EQ(expected_transposed_graph, actual_transposed_graph);
}

auto parse_job(std::string &job) -> std::tuple<std::optional<uint64_t>, bool> {
    bool end_of_chain = false;
    if (job.at(job.size() - 1) == ';') {
        job.resize(job.size() - 1);
        end_of_chain = true;
    }

    std::optional<uint64_t> cost = std::nullopt;
    if (job.at(job.size() - 1) == ')') {
        auto       res      = job.rfind('(', job.size() - 1);
        const auto str_cost = job.substr(res + 1, job.size() - 1);
        cost                = std::stoull(str_cost);
        job.resize(res);
    }
    return {cost, end_of_chain};
}

auto parse_input(std::istream &input)
    -> std::tuple<std::vector<std::vector<uint64_t>>,
                  std::unordered_map<uint64_t, uint64_t>,
                  std::unordered_map<uint64_t, std::string>,
                  uint64_t> {
    std::vector<uint64_t>                     chain;
    std::vector<std::vector<uint64_t>>        chains;

    uint64_t                                  seen_names_count = 0;
    std::unordered_map<std::string, uint64_t> name2index;
    std::unordered_map<uint64_t, std::string> index2name;
    std::unordered_map<uint64_t, uint64_t>    index2cost;
    while (!input.eof()) {
        std::string s;
        input >> s;

        const auto [cost_opt, is_end_of_chain] = parse_job(s);
        if (cost_opt.has_value()) {
            const auto cost        = cost_opt.value();

            auto       found_index = name2index.find(s);
            uint64_t   index;
            if (found_index == name2index.end()) {
                index = name2index[s] = seen_names_count++;
                index2name[index]     = s;
            } else {
                index = found_index->second;
            }
            index2cost[index] = cost;
        }

        const auto index = name2index[s];
        chain.push_back(index);
        if (is_end_of_chain) {
            chains.push_back(chain);
            chain.clear();
            continue;
        }
        input >> s;
    }
    chains.emplace_back(std::move(chain));
    return {chains, index2cost, index2name, seen_names_count};
}

auto construct_oriented_graph(
    const std::vector<std::vector<uint64_t>>     &chains,
    const std::unordered_map<uint64_t, uint64_t> &index2cost,
    const uint64_t graph_order) -> WeightedAdjListT {
    WeightedAdjListT graph(graph_order);
    for (const auto &chain : chains) {
        if (chain.size() <= 1) {
            continue;
        }
        auto prev_job_iter    = chain.begin();
        auto current_job_iter = std::next(chain.begin());
        do {
            const auto prev_job      = *prev_job_iter;
            const auto current_job   = *current_job_iter;
            const auto job_cost_pair = index2cost.find(current_job);
            assert(job_cost_pair != index2cost.end());
            const auto job_cost = job_cost_pair->second;
            graph.at(prev_job).emplace(current_job, job_cost);

            ++prev_job_iter;
            ++current_job_iter;
        } while (current_job_iter != chain.end());
    }
    return graph;
}

auto color_red(uint64_t                start,
               const AdjListT         &transposed_graph,
               std::vector<NodeColor> &vertex_colors) -> void {
    vertex_colors.at(start) = NodeColor::RED;
    for (const auto parent : transposed_graph.at(start)) {
        color_red(parent, transposed_graph, vertex_colors);
    }
}

auto mark_critical_path(const AdjListT         &transposed_graph,
                        std::vector<NodeColor> &vertex_colors,
                        const std::vector<std::optional<uint64_t>> &max_costs) {

    uint64_t max_total_cost  = 0;
    uint64_t max_cost_vertex = 0;
    for (uint64_t vertex = 0; vertex < max_costs.size(); ++vertex) {
        const auto vertex_max_cost_opt = max_costs.at(vertex);
        if (!vertex_max_cost_opt.has_value()) {
            continue;
        }
        const auto vertex_max_cost = vertex_max_cost_opt.value();
        if (max_total_cost < vertex_max_cost) {
            max_total_cost  = vertex_max_cost;
            max_cost_vertex = vertex;
        }
    }

    color_red(max_cost_vertex, transposed_graph, vertex_colors);
}

auto graph2dotlang(const WeightedAdjListT                          &graph,
                   const std::vector<NodeColor>                    &colors,
                   const std::unordered_map<uint64_t, uint64_t>    &index2cost,
                   const std::unordered_map<uint64_t, std::string> &index2name)
    -> std::string {
    // stringstream'ом пользоваться почему-то нельзя :(
    std::string res;
    res += "digraph {\n";
    for (uint64_t vertex = 0; vertex < graph.size(); ++vertex) {
        const auto  cost = index2cost.at(vertex);
        const auto &name = index2name.at(vertex);

        res += name + "[label = \"" + name + "(" + std::to_string(cost) + ")" +
               "\"";
        if (colors.at(vertex) == NodeColor::RED) {
            res += ", color = red";
        } else if (colors.at(vertex) == NodeColor::BLUE) {
            res += ", color = blue";
        }
        res += "]\n";
    }

    for (uint64_t parent = 0; parent < graph.size(); ++parent) {
        const auto &parent_name = index2name.at(parent);
        for (const auto [child, _] : graph.at(parent)) {
            const auto &child_name  = index2name.at(child);
            res                    += parent_name + " -> " + child_name;
            if (colors.at(parent) == NodeColor::RED &&
                colors.at(child) == NodeColor::RED) {
                res += " [color = red]";
            } else if (colors.at(parent) == NodeColor::BLUE &&
                       colors.at(child) == NodeColor::BLUE) {
                res += " [color = blue]";
            }
            res += '\n';
        }
    }

    res += '}';
    return res;
}

auto main(int argc, char *argv[]) -> int {
    assert(argc == 2);
    std::ifstream foo(argv[1]);

    const auto [chains, index2cost, index2name, graph_order] = parse_input(foo);

    const auto graph =
        construct_oriented_graph(chains, index2cost, graph_order);

    auto       res           = TarjanTraverser::build(graph);
    auto       vertex_colors = std::get<0>(res);
    const auto not_bases     = std::get<1>(res);

    const auto [max_costs, trimmed_transposed_graph] =
        DijkstraTraverser::traverse(graph, vertex_colors, not_bases);
    mark_critical_path(trimmed_transposed_graph, vertex_colors, max_costs);
    const auto graph_repr =
        graph2dotlang(graph, vertex_colors, index2cost, index2name);
    std::cout << graph_repr << std::endl;
}
