#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <sys/types.h>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using AdjListT = std::vector<std::vector<uint64_t>>;

class TarjanTraverser {
 public:
    static auto traverse(const AdjListT &graph) -> uint64_t {
        TarjanTraverser traverser(graph);
        for (uint64_t vertex = 0; vertex < graph.size(); ++vertex) {
            if (!traverser.visited_.at(vertex)) {
                traverser.traverse_vertex(vertex);
            }
        }
        return traverser.natural_cycles_count_;
    }

 private:
    const AdjListT       &graph_;
    std::vector<bool>     visited_;
    std::vector<bool>     on_stack_;
    std::stack<uint64_t>  stack_;
    std::vector<uint64_t> in_time_;
    std::vector<uint64_t> low_;
    uint64_t              timestamp_{0};
    uint64_t              natural_cycles_count_{0};

    auto                  traverse_vertex(uint64_t start) -> void {
        on_stack_.at(start) = true;
        visited_.at(start)  = true;
        stack_.push(start);
        in_time_.at(start) = low_.at(start) = timestamp_++;

        for (const auto neighbour : graph_.at(start)) {
            if (!visited_.at(neighbour)) {
                traverse_vertex(neighbour);
            }
            if (on_stack_.at(neighbour)) {
                low_.at(start) = std::min(low_.at(start), low_.at(neighbour));
            }
        }

        if (low_.at(start) == in_time_.at(start)) {
            if (const auto top_stack_item = stack_.top();
                top_stack_item == start) {
                stack_.pop();
                return;
            }
            for (;;) {
                const auto top_stack_item = stack_.top();
                stack_.pop();
                if (top_stack_item == start) {
                    break;
                }
            }
            ++natural_cycles_count_;
        }
    }

    explicit TarjanTraverser(const AdjListT &graph)
        : graph_(graph), visited_(graph_.size()), on_stack_(graph_.size()),
          in_time_(graph_.size()), low_(graph_.size()) {
    }
};

auto process(uint64_t                                current_label_index,
             uint64_t                               &prev_label_index,
             AdjListT                               &graph,
             std::unordered_map<uint64_t, uint64_t> &label2index,
             uint64_t                               &total_label_count,
             std::set<uint64_t, std::greater<>>     &forward_jumps,
             std::istream                           &is) -> uint64_t {
    std::string command;
    is >> command;

    graph.at(prev_label_index).push_back(current_label_index);

    if (command == "ACTION") {
        prev_label_index = current_label_index;
    } else if (command == "BRANCH") {
        uint64_t branch_jump_label;
        is >> branch_jump_label;

        uint64_t branch_jump_label_index;
        if (const auto found = label2index.find(branch_jump_label);
            found == label2index.end()) {
            label2index[branch_jump_label] = branch_jump_label_index =
                total_label_count++;
            forward_jumps.insert(branch_jump_label_index);
        } else {
            branch_jump_label_index = found->second;
        }

        graph.at(current_label_index).push_back(branch_jump_label_index);
        prev_label_index = current_label_index;
    } else if (command == "JUMP") {
        uint64_t jump_label;
        is >> jump_label;

        uint64_t jump_destination = 0;
        if (const auto found = label2index.find(jump_label);
            found == label2index.end()) {
            // Прыжок вперед
            prev_label_index        = current_label_index;
            label2index[jump_label] = total_label_count;
            jump_destination        = total_label_count;
            ++total_label_count;
            forward_jumps.insert(jump_destination);
        } else {
            // Прыжок вперед на известную метку (может как вперед, так и назад)
            jump_destination             = found->second;
            const auto found_destination = forward_jumps.find(jump_destination);
            if (found_destination == forward_jumps.end()) {
                graph.at(current_label_index).push_back(jump_destination);
                return current_label_index;
            }
        }
        for (;;) {
            uint64_t skipping_label;
            is >> skipping_label;

            if (const auto found_skipping = label2index.find(skipping_label);
                found_skipping != label2index.end()) {
                const uint64_t skipping_label_index = found_skipping->second;
                forward_jumps.erase(forward_jumps.begin());

                prev_label_index = current_label_index;
                if (skipping_label_index == jump_destination) {
                    return process(jump_destination,
                                   prev_label_index,
                                   graph,
                                   label2index,
                                   total_label_count,
                                   forward_jumps,
                                   is);
                }
            }
            std::string placeholder;
            std::getline(is, placeholder);
        }
    }
    uint64_t next_label;
    is >> next_label;
    if (!is) {
        return prev_label_index;
    }

    uint64_t next_label_index;
    if (const auto found = label2index.find(next_label);
        found == label2index.end()) {
        label2index[next_label] = next_label_index = total_label_count++;
    } else {
        next_label_index = found->second;
    }

    return next_label_index;
}

auto main() -> int {
    std::ifstream foo;
    foo.open("/home/blackdeer/projects/discrete/test.txt");

    uint64_t n;
    foo >> n;
    AdjListT graph(n);

    uint64_t first_label;
    foo >> first_label;
    std::unordered_map<uint64_t, uint64_t> label2index{
        {first_label, 0}
    };
    uint64_t                           total_label_count   = 1;
    uint64_t                           current_label_index = 0;
    uint64_t                           prev_label_index    = 0;

    std::set<uint64_t, std::greater<>> forward_jumps;
    for (;;) {
        const auto next_label = process(current_label_index,
                                        prev_label_index,
                                        graph,
                                        label2index,
                                        total_label_count,
                                        forward_jumps,
                                        foo);
        if (next_label == prev_label_index) {
            break;
        }
        prev_label_index    = current_label_index;
        current_label_index = next_label;
    }

    const auto res = TarjanTraverser::traverse(graph);
    std::cout << res << std::endl;
    return 0;
}
