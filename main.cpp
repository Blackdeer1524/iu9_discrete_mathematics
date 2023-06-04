#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

// https://cp-algorithms.com/graph/bridge-searching.html#algorithm

int n,
    m;                              // number of nodes
std::vector<std::vector<int>> adj;  // adjacency list of graph

std::vector<bool>             visited;
std::vector<int>              tin, low;
int                           timer;

auto                          dfs(int current, int parrent = -1) -> uint64_t {
    uint64_t count   = 0;
    visited[current] = true;
    tin[current] = low[current] = timer++;
    for (int to : adj[current]) {
        if (to == parrent) {
            continue;
        }
        if (visited[to]) {
            low[current] = std::min(low[current], tin[to]);
        } else {
            count += dfs(to, current);
            low[current] = std::min(low[current], low[to]);
            if (low[to] > tin[current]) {
                ++count;
            }
        }
    }
    return count;
}

auto find_bridges() -> uint64_t {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    uint64_t count = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            count += dfs(i);
        }
    }
    return count;
}

auto main() -> int {
    std::cin >> n >> m;
    adj.resize(n);

    for (int i = 0; i < m; ++i) {
        int u;
        int v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    auto res = find_bridges();
    std::cout << res;
}
