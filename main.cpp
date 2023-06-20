#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

struct Vertex {
    bool                  is_unreachable;
    uint64_t              tin;
    std::string           command;
    uint64_t              operand;
    Vertex               *parent;
    Vertex               *ancestor;
    Vertex               *idom;
    Vertex               *sdom;
    Vertex               *label;
    std::vector<Vertex *> next;
    std::vector<Vertex *> prev;
    std::vector<Vertex *> bucket;
};

uint64_t timestamp = 1;

auto     dfs(Vertex *v) -> void {
    v->is_unreachable = false;
    v->tin            = timestamp++;
    for (uint64_t child_i = 0; child_i < v->next.size(); ++child_i) {
        if (v->next.at(child_i)->is_unreachable) {
            v->next.at(child_i)->parent = v;
            dfs(v->next.at(child_i));
        }
    }
}

auto find_min(Vertex *v) -> Vertex * {
    if (v->ancestor == nullptr) {
        return v;
    }
    std::stack<Vertex *> s;
    auto                *u = v;
    while (u->ancestor->ancestor != nullptr) {
        s.push(u);
        u = u->ancestor;
    }
    while (!s.empty()) {
        v = s.top();
        s.pop();
        if (v->ancestor->label->sdom->tin < v->label->sdom->tin) {
            v->label = v->ancestor->label;
        }
        v->ancestor = u->ancestor;
    }
    return v->label;
}

auto dominators(std::vector<Vertex *> &graph) -> void {
    std::sort(graph.begin(),
              graph.end(),
              [](const Vertex *left, const Vertex *right) -> bool {
                  return left->tin > right->tin;
              });
    for (auto *vertex : graph) {
        if (vertex->tin == 1) {
            continue;
        }

        for (auto *v : vertex->prev) {
            auto *u = find_min(v);
            if (u->sdom->tin < vertex->sdom->tin) {
                vertex->sdom = u->sdom;
            }
        }

        vertex->ancestor = vertex->parent;
        vertex->sdom->bucket.push_back(vertex);
        for (auto *v : vertex->parent->bucket) {
            auto *u = find_min(v);
            if (u->sdom == v->sdom) {
                v->idom = v->sdom;
            } else {
                v->idom = u;
            }
        }
        vertex->parent->bucket.clear();
    }

    for (auto *vertex : graph) {
        if (vertex->tin == 1) {
            continue;
        }
        if (vertex->idom != vertex->sdom) {
            vertex->idom = vertex->idom->idom;
        }
    }

    graph.at(graph.size() - 1)->idom = nullptr;
}

auto main() -> int {
    uint64_t    n;
    uint64_t    label;
    uint64_t    operand;
    std::string command;
    std::cin >> n;

    std::vector<Vertex *>                  graph;
    std::unordered_map<uint64_t, uint64_t> label2index;
    for (uint64_t i = 0; i < n; ++i) {
        auto *v           = new Vertex{};
        v->is_unreachable = true;
        v->sdom           = v;
        v->label          = v;
        graph.push_back(v);
    }

    for (uint64_t i = 0; i < graph.size(); ++i) {
        std::cin >> label >> command;
        graph.at(i)->command = command;
        if (command != "ACTION") {
            std::cin >> operand;
            graph.at(i)->operand = operand;
        }
        label2index[label] = i;
    }

    for (uint64_t i = 0; i < graph.size(); ++i) {
        auto *vertex = graph.at(i);
        if (vertex->command == "ACTION") {
            if (i < n - 1) {
                graph.at(i)->next.push_back(graph.at(i + 1));
                graph.at(i + 1)->prev.push_back(graph.at(i));
            }
        } else if (vertex->command == "JUMP") {
            operand = label2index[graph.at(i)->operand];
            graph.at(i)->next.push_back(graph.at(operand));
            graph.at(operand)->prev.push_back(graph.at(i));
        } else {
            operand = label2index[graph.at(i)->operand];
            graph.at(i)->next.push_back(graph.at(operand));
            graph.at(operand)->prev.push_back(graph.at(i));
            if (i < n - 1) {
                graph.at(i)->next.push_back(graph.at(i + 1));
                graph.at(i + 1)->prev.push_back(graph.at(i));
            }
        }
    }

    dfs(graph[0]);

    for (auto &ancestor : graph) {
        if (ancestor->is_unreachable) {
            continue;
        }
        for (uint64_t j = 0; j < ancestor->prev.size(); ++j) {
            if (!ancestor->prev.at(j)->is_unreachable) {
                continue;
            }
            ancestor->prev.at(j) = ancestor->prev.at(ancestor->prev.size() - 1);
            ancestor->prev.resize(ancestor->prev.size() - 1);
            j--;
        }
    }

    for (uint64_t i = 0; i < graph.size(); ++i) {
        if (!graph.at(i)->is_unreachable) {
            continue;
        }
        delete graph.at(i);
        graph.at(i) = graph.at(graph.size() - 1);
        graph.resize(graph.size() - 1);
        i--;
        n--;
    }

    dominators(graph);

    uint64_t ans = 0;
    for (auto *vertex : graph) {
        for (auto *p : vertex->prev) {
            while (p != vertex && p != nullptr) {
                p = p->idom;
            }
            if (p == vertex) {
                ans++;
                break;
            }
        }
    }
    std::cout << ans << std::endl;

    for (auto *vertex : graph) {
        delete vertex;
    }
}
