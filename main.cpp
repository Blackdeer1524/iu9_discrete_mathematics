#include <cstdint>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using transitions_table = std::vector<std::vector<uint64_t>>;
using outputs_table     = std::vector<std::vector<uint64_t>>;

struct Automaton {
    transitions_table delta;
    outputs_table     phi;
    uint64_t          states_count;
    uint64_t          alphabet_size;
};

auto create_name(uint64_t q, const std::string &y) -> std::string {
    return "(" + std::to_string(q) + "," + y + ")";
}

auto main() -> int {
    std::istream &in = std::cin;

    uint64_t      in_alphabet_size;
    in >> in_alphabet_size;
    std::vector<std::string> in_alphabet(in_alphabet_size);
    for (uint64_t i = 0; i < in_alphabet_size; ++i) {
        std::string placeholder;
        in >> placeholder;
        in_alphabet.at(i) = std::move(placeholder);
    }

    uint64_t out_alphabet_size;
    in >> out_alphabet_size;
    std::vector<std::string> out_alphabet(out_alphabet_size);
    for (uint64_t i = 0; i < out_alphabet_size; ++i) {
        std::string placeholder;
        in >> placeholder;
        out_alphabet.at(i) = std::move(placeholder);
    }

    uint64_t states_count;
    in >> states_count;

    transitions_table delta(states_count,
                            std::vector<uint64_t>(in_alphabet_size));
    outputs_table phi(states_count, std::vector<uint64_t>(in_alphabet_size));

    for (uint64_t i = 0; i < states_count; ++i) {
        for (uint64_t j = 0; j < in_alphabet_size; ++j) {
            uint64_t next_state;
            in >> next_state;
            delta.at(i).at(j) = next_state;
        }
    }

    for (uint64_t i = 0; i < states_count; ++i) {
        for (uint64_t j = 0; j < in_alphabet_size; ++j) {
            uint64_t output;
            in >> output;
            phi.at(i).at(j) = output;
        }
    }

    Automaton a{delta, phi, states_count, in_alphabet_size};

    std::vector<std::unordered_set<uint64_t>> z(states_count);
    for (uint64_t i = 0; i < a.states_count; ++i) {
        for (uint64_t j = 0; j < a.alphabet_size; ++j) {
            const auto next_state = a.delta.at(i).at(j);
            const auto output     = a.phi.at(i).at(j);
            z.at(next_state).insert(output);
        }
    }

    std::cout << "digraph {" << std::endl;
    std::cout << "    rankdir = LR" << std::endl;

    std::unordered_map<std::string, uint64_t> moore_state_name2index;
    uint64_t                                  moore_states_count = 0;
    for (uint64_t q = 0; q < z.size(); ++q) {
        for (const auto y_i : z.at(q)) {
            const auto &y                = out_alphabet.at(y_i);
            const auto  name             = create_name(q, y);
            moore_state_name2index[name] = moore_states_count;
            std::cout << "    " << moore_states_count
                      << "[label = \"" + name + "\"]" << std::endl;
            ++moore_states_count;
        }
    }

    for (uint64_t z_class_i = 0; z_class_i < a.states_count; ++z_class_i) {
        const auto z_class = z.at(z_class_i);
        for (uint64_t in_signal_i = 0; in_signal_i < a.alphabet_size;
             ++in_signal_i) {
            const auto  in_signal       = in_alphabet.at(in_signal_i);

            const auto  next_state      = a.delta.at(z_class_i).at(in_signal_i);
            const auto  output_signal_i = a.phi.at(z_class_i).at(in_signal_i);
            const auto &output          = out_alphabet.at(output_signal_i);
            const auto  next_moore_state = create_name(next_state, output);
            const auto  next_moore_state_i =
                moore_state_name2index.at(next_moore_state);
            for (const auto y_i : z_class) {
                const auto &y               = out_alphabet.at(y_i);
                const auto  cur_moore_state = create_name(z_class_i, y);
                const auto  cur_moore_state_i =
                    moore_state_name2index.at(cur_moore_state);
                std::cout << "    " << cur_moore_state_i << " -> "
                          << next_moore_state_i
                          << " [label = \"" + in_signal + "\"]" << std::endl;
            }
        }
    }
    std::cout << "}" << std::endl;
}
