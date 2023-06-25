#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

constexpr static uint64_t LAMBDA_INDEX = 0;

using nondet_transitions = std::vector<std::vector<std::vector<uint64_t>>>;

class DFSTraverser {
 public:
    explicit DFSTraverser(const nondet_transitions &delta)
        : delta_(delta), visited_(delta.size()) {
    }

    // Достижимые вершины (включая start)
    // (λ не "съедает" символ)
    auto closure(const uint64_t start) -> std::vector<uint64_t> {
        visited_.assign(visited_.size(), false);
        reachable_.clear();

        fill_reachable(start);

        return reachable_;
    }

    auto closure(const std::vector<uint64_t> &start) -> std::vector<uint64_t> {
        visited_.assign(visited_.size(), false);
        reachable_.clear();

        for (const uint64_t state : start) {
            fill_reachable(state);
        }

        return reachable_;
    }

 private:
    const nondet_transitions &delta_;
    std::vector<uint64_t>     reachable_;
    std::vector<bool>         visited_;

    auto                      fill_reachable(uint64_t start) -> void {
        visited_.at(start) = true;
        reachable_.push_back(start);

        const auto &next_candidates = delta_.at(start).at(LAMBDA_INDEX);
        for (const uint64_t next_state : next_candidates) {
            if (visited_.at(next_state)) {
                continue;
            }
            fill_reachable(next_state);
        }
    }
};

auto vect2string(const std::vector<uint64_t> &states,
                 const std::string           &sep = " ") -> std::string {
    if (states.empty()) {
        return "";
    }

    std::string res;
    for (uint64_t i = 0; i < states.size() - 1; ++i) {
        res += std::to_string(states.at(i)) + sep;
    }
    res += std::to_string(states.at(states.size() - 1));
    return res;
}

auto vect2string(const std::vector<std::string> &states,
                 const std::string              &sep = " ") -> std::string {
    if (states.empty()) {
        return "";
    }

    std::string res;
    for (uint64_t i = 0; i < states.size() - 1; ++i) {
        res += states.at(i) + sep;
    }
    res += states.at(states.size() - 1);
    return res;
}

auto det(const nondet_transitions &delta,
         const std::vector<bool>  &final,
         uint64_t                  q)
    -> std::tuple<std::vector<std::vector<std::optional<uint64_t>>>,
                  std::vector<bool>,
                  std::vector<std::string>> {
    DFSTraverser                                      traverser(delta);
    const std::vector<uint64_t>                       q0 = traverser.closure(q);

    std::vector<std::vector<std::optional<uint64_t>>> det_delta;
    std::vector<bool>                                 det_is_final;

    std::stack<std::tuple<std::vector<uint64_t>, uint64_t>> s;
    const auto q0_label = vect2string(q0);
    s.emplace(q0, 0);
    std::unordered_map<std::string, uint64_t> state_label2index{
        {q0_label, 0}
    };
    uint64_t det_states_count = 1;

    while (!s.empty()) {
        const auto [reachable_states, det_state_index] = s.top();
        det_is_final.resize(std::max(det_is_final.size(), det_state_index + 1));
        s.pop();

        for (const uint64_t reachable_state : reachable_states) {
            if (final.at(reachable_state)) {
                det_is_final.at(det_state_index) = true;
                break;
            }
        }

        for (uint64_t in_symbol_i = LAMBDA_INDEX + 1;
             in_symbol_i < delta.at(0).size();
             ++in_symbol_i) {
            std::vector<uint64_t> next_states_candidates;
            for (const uint64_t reachable_state : reachable_states) {
                const auto &neighbours =
                    delta.at(reachable_state).at(in_symbol_i);
                std::copy(neighbours.begin(),
                          neighbours.end(),
                          std::back_inserter(next_states_candidates));
            }
            std::vector<uint64_t> next_states =
                traverser.closure(next_states_candidates);
            std::sort(next_states.begin(), next_states.end());

            const auto next_state_label = vect2string(next_states);
            uint64_t   next_state_index;
            if (const auto found = state_label2index.find(next_state_label);
                found == state_label2index.end()) {
                next_state_index = det_states_count;
                ++det_states_count;
                state_label2index.insert({next_state_label, next_state_index});
                s.emplace(next_states, next_state_index);
            } else {
                next_state_index = found->second;
            }

            det_delta.resize(std::max(det_delta.size(), det_state_index + 1));
            det_delta.at(det_state_index)
                .resize(std::max(det_delta.at(det_state_index).size(),
                                 in_symbol_i + 1));

            det_delta.at(det_state_index).at(in_symbol_i) = next_state_index;
        }
    }

    std::vector<std::string> index2state_label;
    index2state_label.resize(state_label2index.size());
    for (const auto &[key, index] : state_label2index) {
        index2state_label.at(index) = (key);
    }

    return {det_delta, det_is_final, index2state_label};
}

auto print_automaton(
    const std::vector<std::string>                          &alphabet,
    const std::vector<std::vector<std::optional<uint64_t>>> &delta,
    const std::vector<bool>                                 &final,
    const std::vector<std::string> &index2name) -> void {
    std::cout << "digraph {" << std::endl;
    std::cout << "    rankdir = LR" << std::endl;
    for (uint64_t state_i = 0; state_i < index2name.size(); ++state_i) {
        const auto &state_label = index2name.at(state_i);
        std::cout << "    " << std::to_string(state_i)
                  << " [label = \"[" + state_label + "]\", shape = ";
        if (final.at(state_i)) {
            std::cout << "doublecircle]" << std::endl;
        } else {
            std::cout << "circle]" << std::endl;
        }
    }
    for (uint64_t cur_state_i = 0; cur_state_i < delta.size(); ++cur_state_i) {
        std::unordered_map<uint64_t, std::vector<std::string>> arrows;
        for (uint64_t in_symbol_i = 0; in_symbol_i < delta.at(0).size();
             ++in_symbol_i) {
            const auto next_state_opt = delta.at(cur_state_i).at(in_symbol_i);
            if (!next_state_opt.has_value()) {
                continue;
            }
            const auto  next_state_i = next_state_opt.value();
            const auto &in_symbol    = alphabet.at(in_symbol_i);
            arrows[next_state_i].push_back(in_symbol);
        }
        for (const auto &[next_state_i, in_symbols] : arrows) {
            std::cout << "    " << cur_state_i << " -> " << next_state_i
                      << " [label = \"" + vect2string(in_symbols, ", ") + "\"]"
                      << std::endl;
        }
    }
    std::cout << "}" << std::endl;
}

auto main() -> int {
    uint64_t states_count;
    uint64_t transitions_count;
    std::cin >> states_count >> transitions_count;

    std::vector<std::vector<std::vector<uint64_t>>> delta(
        states_count, std::vector<std::vector<uint64_t>>(1));
    std::unordered_map<std::string, uint64_t> symbol2index{
        {"lambda", LAMBDA_INDEX},
    };
    uint64_t alphabet_size = 1;  // 0 уже заняли лямбдой

    for (uint64_t i = 0; i < transitions_count; ++i) {
        uint64_t    from;
        uint64_t    to;
        std::string transition_signal;
        std::cin >> from >> to >> transition_signal;
        uint64_t transition_signal_index;
        if (const auto found = symbol2index.find(transition_signal);
            found == symbol2index.end()) {
            symbol2index.emplace(transition_signal, alphabet_size);
            transition_signal_index = alphabet_size;
            ++alphabet_size;
        } else {
            transition_signal_index = found->second;
        }
        delta.at(from).resize(alphabet_size);
        delta.at(from).at(transition_signal_index).push_back(to);
    }

    std::vector<bool> final(states_count);
    for (uint64_t i = 0; i < states_count; ++i) {
        delta.at(i).resize(alphabet_size);

        int is_final;
        std::cin >> is_final;
        final.at(i) = is_final;
    }

    uint64_t q0;
    std::cin >> q0;

    std::vector<std::string> alphabet(symbol2index.size());
    for (const auto &[symbol, index] : symbol2index) {
        alphabet.at(index) = symbol;
    }

    const auto [det_delta, det_final, det_index2label] = det(delta, final, q0);
    print_automaton(alphabet, det_delta, det_final, det_index2label);
}
