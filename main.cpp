#include <algorithm>
#include <bits/iterator_concepts.h>
#include <cstdint>
#include <forward_list>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <vector>

struct Vertex;

struct AdjecencyListItem {
    AdjecencyListItem(uint64_t weight, Vertex *vertex)
        : weight_(weight), vertex_(vertex) {
    }

    uint64_t weight_;
    Vertex *vertex_;  // AdjecencyListItem НЕ владеет указателем =>
                      // нет delete
};

struct Vertex {
 public:
    static auto connect(Vertex *one, Vertex *other, uint64_t weight) -> void {
        auto *adj_node_for_first_vertex = new AdjecencyListItem(weight, other);
        one->adj_.push_front(adj_node_for_first_vertex);

        auto *adj_node_for_second_vertex = new AdjecencyListItem(weight, one);
        other->adj_.push_front(adj_node_for_second_vertex);
    }

    uint64_t key_ = std::numeric_limits<decltype(key_)>::max();
    std::forward_list<AdjecencyListItem *> adj_;
    Vertex                                *parent_      = nullptr;

    bool                                   inside_heap_ = true;

    Vertex()                                            = default;
    Vertex(const Vertex &)                              = delete;
    Vertex(Vertex &&)                                   = default;
    auto operator=(const Vertex &) -> Vertex          & = delete;
    auto operator=(Vertex &&) -> Vertex               & = default;

    ~Vertex() {
        for (auto *list_item : adj_) {
            delete list_item;
        }
    }

    auto operator<(const Vertex &other) const -> bool {
        return key_ > other.key_;
    }
};

auto prim(std::vector<Vertex *> heap) -> uint64_t {
    static auto cmp = [](const auto &left, const auto &right) {
        return left->key_ > right->key_;
    };

    heap.front()->key_ = 0;
    uint64_t res       = 0;
    while (!heap.empty()) {
        auto *vertex = heap.front();
        std::pop_heap(heap.begin(), heap.end(), cmp);
        heap.resize(heap.size() - 1);
        vertex->inside_heap_  = false;
        res                  += vertex->key_;
        for (auto *adj_list_item : vertex->adj_) {
            auto  weight     = adj_list_item->weight_;
            auto *adj_vertex = adj_list_item->vertex_;
            if (adj_vertex->inside_heap_ && weight < adj_vertex->key_) {
                adj_vertex->key_    = weight;
                adj_vertex->parent_ = vertex;

                auto iter = std::find(heap.begin(), heap.end(), adj_vertex);
                std::make_heap(heap.begin(), ++iter, cmp);
            }
        }
    }
    return res;
}

auto main() -> int {
    uint64_t graph_order;
    uint64_t graph_size;
    std::cin >> graph_order >> graph_size;

    auto vertecies = std::vector<Vertex *>(graph_order);
    for (uint64_t i = 0; i < graph_order; ++i) {
        vertecies[i] = new Vertex();
    }

    for (uint64_t i = 0; i < graph_size; ++i) {
        uint64_t start;
        uint64_t end;
        uint64_t weight;
        std::cin >> start >> end >> weight;
        Vertex::connect(vertecies[start], vertecies[end], weight);
    }
    auto res = prim(vertecies);

    for (uint64_t i = 0; i < graph_order; ++i) {
        delete vertecies[i];
    }
    std::cout << res;
}
