#include <algorithm>
#include <cstdint>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <vector>

using AdjecencyListT = std::vector<std::vector<uint64_t>>;

std::vector<bool>     visited;
std::vector<uint64_t> toposorted_vertices;

auto dfs(const AdjecencyListT &adj_list, const uint64_t start) -> void {
    if (visited[start]) {
        return;
    }
    visited[start] = true;
    for (const auto neighbour : adj_list[start]) {
        dfs(adj_list, neighbour);
    }
    toposorted_vertices.push_back(start);
}

auto solve(const AdjecencyListT &adj_list) -> std::vector<bool> {
    const uint64_t    order                          = adj_list.size();
    uint64_t          max_components_order           = 0;
    uint64_t          max_components_size            = 0;
    uint64_t          max_components_min_vertex_name = order;
    std::vector<bool> is_max_component;

    std::vector<bool> seen_vertecies = std::vector<bool>(visited.size(), false);
    for (uint64_t dfs_start = 0; dfs_start < seen_vertecies.size();
         ++dfs_start) {
        if (seen_vertecies[dfs_start]) {
            continue;
        }
        dfs(adj_list, dfs_start);

        uint64_t components_size            = 0;
        uint64_t components_order           = toposorted_vertices.size();
        uint64_t components_min_vertex_name = order;
        for (const auto vertex : toposorted_vertices) {
            components_size += adj_list[vertex].size();
            components_min_vertex_name =
                std::min(components_min_vertex_name, vertex);
        }
        components_size >>= 1;

        if (max_components_order < components_order ||
            (max_components_order == components_order &&
             (max_components_size < components_size ||
              (max_components_size == components_size &&
               max_components_min_vertex_name > components_min_vertex_name)))) {

            max_components_order           = components_order;
            max_components_size            = components_size;
            max_components_min_vertex_name = components_min_vertex_name;
            is_max_component               = visited;
        }

        for (uint64_t i = 0; i < visited.size(); ++i) {
            seen_vertecies[i] = visited[i] || seen_vertecies[i];
        }

        visited.assign(visited.size(), false);
        toposorted_vertices.clear();
    }
    return is_max_component;
}

auto print_graph(const AdjecencyListT    &adjacency_list,
                 const std::vector<bool> &is_max_component) -> void {
    auto adj_list = adjacency_list;

    // std::stringstream ss;
    std::cout << "graph G {" << std::endl;

    for (uint64_t vertex = 0; vertex < adj_list.size(); ++vertex) {
        std::cout << vertex;
        if (is_max_component[vertex]) {
            std::cout << " [color=red];";
        }
        std::cout << std::endl;
    }

    for (uint64_t vertex = 0; vertex < adj_list.size(); ++vertex) {
        for (const auto neighbour : adj_list[vertex]) {
            std::cout << vertex << " -- " << neighbour;
            if (is_max_component[vertex]) {
                std::cout << " [color=red]";
            }
            std::cout << ';' << std::endl;

            // Удаляем с конца для O(1)
            auto item = std::find(adj_list[neighbour].rbegin(),
                                  adj_list[neighbour].rend(),
                                  vertex);
            adj_list[neighbour].erase(std::next(item).base());
        }
    }

    std::cout << "}";
}

auto main() -> int {
    uint64_t order;
    uint64_t size;
    std::cin >> order >> size;
    visited.resize(order);

    AdjecencyListT adj_list(order);
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t from;
        uint64_t to;

        std::cin >> from >> to;

        adj_list[from].push_back(to);
        adj_list[to].push_back(from);
    }

    for (uint64_t i = 0; i < order; ++i) {
        std::sort(
            adj_list[i].begin(), adj_list[i].end(), std::greater<uint64_t>());
    }

    auto is_max_component = solve(adj_list);
    print_graph(adj_list, is_max_component);
}
