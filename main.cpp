#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <ostream>
#include <sstream>
#include <stack>
#include <sys/types.h>
#include <vector>

using timestamp_t = uint64_t;

class AdjecencyList {
 public:
    using edge_color_t                        = bool;
    static constexpr auto DEFAULT_COLOR       = false;
    static constexpr auto MAX_COMPONENT_COLOR = true;

    explicit AdjecencyList(uint64_t N) {
        adj_list_.resize(N);
        edge_colors_.resize(N);
        edge_colors_backup_.resize(N);
    }

    auto push_at(uint64_t index, uint64_t neighbour) {
        adj_list_[index].push_back(neighbour);
        edge_colors_[index].push_back(DEFAULT_COLOR);
        edge_colors_backup_[index].push_back(DEFAULT_COLOR);
    }

    [[nodiscard]] auto size() const -> uint64_t {
        return adj_list_.size();
    }

    [[nodiscard]] auto operator[](uint64_t index) const
        -> const std::vector<uint64_t> & {
        return adj_list_.at(index);
    }

    auto
    recolor_at(uint64_t vertex, uint64_t neighbour_index, edge_color_t color) {
        edge_colors_[vertex][neighbour_index] = color;
    }

    auto restore_coloring() -> void {
        edge_colors_ = edge_colors_backup_;
    }

    auto save_coloring() -> void {
        edge_colors_backup_ = edge_colors_;
    }

    auto clear_color() -> void {
        for (uint64_t i = 0; i < edge_colors_backup_.size(); ++i) {
            for (uint64_t j = 0; j < edge_colors_backup_[i].size(); ++j) {
                edge_colors_[i][j] = DEFAULT_COLOR;
            }
        }
    }

    auto restore_default_color() -> void {
        for (uint64_t i = 0; i < edge_colors_backup_.size(); ++i) {
            for (uint64_t j = 0; j < edge_colors_backup_[i].size(); ++j) {
                edge_colors_backup_[i][j] = edge_colors_[i][j] = DEFAULT_COLOR;
            }
        }
    }

    auto print_dot() -> std::string {
        std::stringstream ss;
        ss << "digraph {" << std::endl;

        std::vector<bool> is_part_of_max_component(adj_list_.size());
        for (uint64_t vertex = 0; vertex < adj_list_.size(); ++vertex) {
            for (uint64_t neighbour_index = 0;
                 neighbour_index < adj_list_[vertex].size();
                 ++neighbour_index) {
                if (edge_colors_[vertex][neighbour_index] ==
                    MAX_COMPONENT_COLOR) {
                    is_part_of_max_component[vertex]                     = true;
                    is_part_of_max_component[adj_list_[vertex]
                                                      [neighbour_index]] = true;
                }
                ss << vertex << " -> " << adj_list_[vertex][neighbour_index];
                if (edge_colors_[vertex][neighbour_index] ==
                    MAX_COMPONENT_COLOR) {
                    ss << "[color=\"red\"]";
                }
                ss << std::endl;
            }
        }

        for (uint64_t vertex = 0; vertex < size(); ++vertex) {
            if (is_part_of_max_component[vertex]) {
                ss << vertex << "[color=\"red\"]" << std::endl;
            }
        }
        ss << "}";
        return ss.str();
    }

 private:
    std::vector<std::vector<uint64_t>>     adj_list_;
    // Отвечают за максимальный компонент:
    // true: является частью макс. компонента
    std::vector<std::vector<edge_color_t>> edge_colors_;
    std::vector<std::vector<edge_color_t>> edge_colors_backup_;
};

enum class Color { WHITE, GREY, BLACK };

std::vector<uint64_t>                ordered_component_vertices;
std::vector<std::optional<uint64_t>> out_time;
std::vector<Color>                   vertex_color;
uint64_t                             timestamp = 0;

auto                                 dfs(const uint64_t start_vertex,
                                         AdjecencyList &adj_list,
                                         const uint64_t dfs_start_time) -> uint64_t {
    uint64_t edge_count        = 0;
    vertex_color[start_vertex] = Color::GREY;

    const auto &neighbourhood = adj_list[start_vertex];
    for (uint64_t neighbour_index = 0; neighbour_index < neighbourhood.size();
         ++neighbour_index) {
        const auto neighbour = neighbourhood[neighbour_index];
        if (out_time[neighbour].has_value() &&
            out_time[neighbour].value() < dfs_start_time) {
            continue;
        }
        adj_list.recolor_at(
            start_vertex, neighbour_index, AdjecencyList::MAX_COMPONENT_COLOR);

        ++edge_count;
        if (vertex_color[neighbourhood[neighbour_index]] == Color::BLACK) {
            continue;
        }
        if (vertex_color[neighbourhood[neighbour_index]] == Color::GREY) {
            continue;
        }
        edge_count +=
            dfs(neighbourhood[neighbour_index], adj_list, dfs_start_time);
    }
    vertex_color[start_vertex] = Color::BLACK;
    out_time[start_vertex]     = timestamp++;
    ordered_component_vertices.emplace_back(start_vertex);
    return edge_count;
}

auto find_max_component(AdjecencyList &graph) -> void {
    AdjecencyList transposed_graph(graph.size());
    for (uint64_t i = 0; i < graph.size(); ++i) {
        for (const auto neighbour : graph[i]) {
            transposed_graph.push_at(neighbour, i);
        }
    }

    for (uint64_t i = 0; i < graph.size(); ++i) {
        if (vertex_color[i] != Color::BLACK) {
            dfs(i, transposed_graph, timestamp);
        }
    }

    transposed_graph.restore_default_color();
    const auto toposorted_vertices = ordered_component_vertices;
    ordered_component_vertices.clear();
    std::fill(vertex_color.begin(), vertex_color.end(), Color::WHITE);
    std::fill(out_time.begin(), out_time.end(), std::nullopt);
    timestamp                               = 0;

    uint64_t max_components_order           = 0;
    uint64_t max_components_edge_count      = 0;
    uint64_t max_components_min_vertex_name = graph.size();
    for (auto backward_iter = toposorted_vertices.rbegin();
         backward_iter != toposorted_vertices.rend();
         backward_iter++) {

        const auto start = *backward_iter;
        if (vertex_color[start] == Color::BLACK) {
            continue;
        }

        auto edge_count = dfs(start, graph, timestamp);
        auto component_min_vertex_name =
            std::accumulate(ordered_component_vertices.begin(),
                            ordered_component_vertices.end(),
                            ordered_component_vertices.size(),
                            [](uint64_t accumulator, uint64_t item) {
                                accumulator = std::min(accumulator, item);
                                return accumulator;
                            });

        if (max_components_order < ordered_component_vertices.size() ||
            max_components_edge_count < edge_count ||
            max_components_min_vertex_name > component_min_vertex_name) {
            graph.save_coloring();
            max_components_order           = ordered_component_vertices.size();
            max_components_edge_count      = edge_count;
            max_components_min_vertex_name = component_min_vertex_name;
        }
        edge_count = 0;
        ordered_component_vertices.clear();
        graph.clear_color();
    }
    graph.restore_coloring();
}

auto main() -> int {
    uint64_t order;
    uint64_t size;
    std::cin >> order >> size;

    AdjecencyList adj_list(order);
    vertex_color.resize(order);
    out_time.resize(order);

    for (uint64_t i = 0; i < size; ++i) {
        uint64_t from;
        uint64_t to;
        std::cin >> from >> to;
        adj_list.push_at(from, to);
    }

    find_max_component(adj_list);
    std::cout << adj_list.print_dot() << std::endl;
}
