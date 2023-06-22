#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using transitions_table = std::vector<std::vector<uint64_t>>;
using signals_table     = std::vector<std::vector<std::string>>;

auto main() -> int {
    uint64_t      n;
    uint64_t      m;
    uint64_t      q0;

    std::istream &in = std::cin;

    in >> n >> m >> q0;

    transitions_table delta(n, std::vector<uint64_t>(m));
    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            uint64_t next_state;
            in >> next_state;
            delta.at(i).at(j) = next_state;
        }
    }

    signals_table phi(n, std::vector<std::string>(m));
    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            std::string result;
            in >> result;
            phi.at(i).at(j) = result;
        }
    }

    constexpr char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    std::cout << "digraph {" << std::endl;
    std::cout << "    rankdir = LR" << std::endl;
    for (uint64_t state_i = 0; state_i < n; ++state_i) {
        for (uint64_t in_signal_i = 0; in_signal_i < m; ++in_signal_i) {
            const auto next_state = delta.at(state_i).at(in_signal_i);
            const auto output     = phi.at(state_i).at(in_signal_i);

            std::cout << "    " << state_i << " -> " << next_state;
            std::cout << " [label = \"" << alphabet[in_signal_i] << "("
                      << output << ")"
                      << "\"]" << std::endl;
        }
    }

    std::cout << "}" << std::endl;
}
