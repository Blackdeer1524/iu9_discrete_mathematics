#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>

using AdjecencyListT = std::vector<std::list<uint64_t>>;

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
    const uint64_t      order                          = adj_list.size();
    uint64_t            max_components_order           = 0;
    uint64_t            max_components_size            = 0;
    uint64_t            max_components_min_vertex_name = order;
    std::vector<bool>   is_max_component;

    std::list<uint64_t> operation_queue;
    for (uint64_t i = 0; i < order; ++i) {
        operation_queue.push_back(i);
    }

    while (!operation_queue.empty()) {
        const auto dfs_start = operation_queue.front();
        operation_queue.pop_front();
        if (visited[dfs_start]) {
            continue;
        }
        dfs(adj_list, dfs_start);

        uint64_t              components_order           = 0;
        uint64_t              components_size            = 0;
        uint64_t              components_min_vertex_name = order;
        std::vector<uint64_t> component;
        for (uint64_t vertex = 0; vertex < order; ++vertex) {
            if (!visited[vertex]) {
                continue;
            }
            components_order++;
            components_size += adj_list[vertex].size();
            components_min_vertex_name =
                std::min(components_min_vertex_name, vertex);
            component.push_back(vertex);
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

        // чтобы не ругался clang-tidy на [[nodiscard]]
        (void)std::remove_if(operation_queue.begin(),
                             operation_queue.end(),
                             [](uint64_t vertex) { return visited[vertex]; });

        toposorted_vertices.clear();
        visited.assign(false, order);
    }
    return is_max_component;
}

auto print_graph(const AdjecencyListT    &adjacency_list,
                 const std::vector<bool> &is_max_component) -> std::string {
    auto              adj_list = adjacency_list;

    std::stringstream ss;
    ss << "graph G {" << std::endl;

    for (uint64_t vertex = 0; vertex < adj_list.size(); ++vertex) {
        if (is_max_component[vertex]) {
            ss << vertex << " [color=red];" << std::endl;
        }
    }

    for (uint64_t vertex = 0; vertex < adj_list.size(); ++vertex) {
        for (const auto neighbour : adj_list[vertex]) {
            ss << vertex << " -- " << neighbour;
            if (is_max_component[vertex]) {
                ss << " [color=red]";
            }
            ss << ';' << std::endl;

            auto item = std::find(
                adj_list[neighbour].begin(), adj_list[neighbour].end(), vertex);
            adj_list[neighbour].erase(item);
        }
    }

    ss << "}";
    return ss.str();
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

    auto is_max_component = solve(adj_list);
    auto res              = print_graph(adj_list, is_max_component);

    std::cout << res << std::endl;
}
