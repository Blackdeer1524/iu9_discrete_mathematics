#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using AdjacencyListT = std::vector<std::vector<uint64_t>>;

struct VertexInfo {
    uint64_t distance = 0;
    bool     is_valid = true;
};

auto bfs(const AdjacencyListT &adj_list, uint64_t start)
    -> std::vector<std::optional<uint64_t>> {
    std::queue<std::pair<uint64_t, uint64_t>> bfs_queue;
    std::vector<bool>                         visited(adj_list.size(), false);
    std::vector<std::optional<uint64_t>>      distances(adj_list.size(),
                                                   std::nullopt);

    bfs_queue.emplace(0, start);
    while (!bfs_queue.empty()) {
        auto [depth, current_vertex] = bfs_queue.front();
        bfs_queue.pop();
        if (visited[current_vertex]) {
            continue;
        }
        visited[current_vertex]   = true;
        distances[current_vertex] = depth;
        for (auto neighbour : adj_list[current_vertex]) {
            if (visited[neighbour]) {
                continue;
            }
            bfs_queue.emplace(depth + 1, neighbour);
        }
    }
    return distances;
}

auto eq_dist_helper(uint64_t                     start_vertex,
                    const AdjacencyListT        &adj_list,
                    const std::vector<uint64_t> &initial_distances,
                    std::vector<bool>           &valid_vertices) -> void {
    std::queue<std::pair<uint64_t, uint64_t>> bfs_queue;
    std::vector<bool>                         visited(adj_list.size(), false);

    bfs_queue.emplace(0, start_vertex);
    while (!bfs_queue.empty()) {
        auto [depth, current_vertex] = bfs_queue.front();
        bfs_queue.pop();
        if (visited[current_vertex]) {
            continue;
        }

        visited[current_vertex] = true;
        if (depth != initial_distances[current_vertex]) {
            valid_vertices[current_vertex] = false;
        }

        for (auto neighbour : adj_list[current_vertex]) {
            if (visited[neighbour]) {
                continue;
            }
            bfs_queue.emplace(depth + 1, neighbour);
        }
    }
}

auto eq_dist(const AdjacencyListT        &adj_list,
             const std::vector<uint64_t> &support_vertices)
    -> std::vector<uint64_t> {
    auto opt_initial_distances = bfs(adj_list, support_vertices.at(0));
    if (std::any_of(opt_initial_distances.begin(),
                    opt_initial_distances.end(),
                    [](auto item) { return !item.has_value(); })) {
        return {};
    }
    auto initial_distances = std::vector<uint64_t>();
    for (const auto item : opt_initial_distances) {
        initial_distances.push_back(item.value());
    }

    auto valid_vertices = std::vector<bool>(adj_list.size(), true);
    for (auto start_vertex : support_vertices) {
        eq_dist_helper(
            start_vertex, adj_list, initial_distances, valid_vertices);
    }
    auto res = std::vector<uint64_t>();
    for (uint64_t i = 0; i < valid_vertices.size(); ++i) {
        if (valid_vertices[i]) {
            res.push_back(i);
        }
    }
    return res;
}

auto main() -> int {
    uint64_t order;
    uint64_t size;
    std::cin >> order >> size;

    auto adj_list = AdjacencyListT(order);
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t from;
        uint64_t to;

        std::cin >> from >> to;
        adj_list[from].push_back(to);
        adj_list[to].push_back(from);
    }
    uint64_t n_support_vertices;
    std::cin >> n_support_vertices;
    auto support_vertices = std::vector<uint64_t>(n_support_vertices);
    for (uint64_t i = 0; i < n_support_vertices; ++i) {
        uint64_t support_vertex;
        std::cin >> support_vertex;
        support_vertices.at(i) = support_vertex;
    }

    auto res = eq_dist(adj_list, support_vertices);
    if (res.empty()) {
        std::cout << '-' << std::endl;
    } else {
        for (auto item : res) {
            std::cout << item << ' ';
        }
    }
}
