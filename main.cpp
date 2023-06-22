#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <optional>
#include <stack>
#include <tuple>
#include <vector>

using transitions_table = std::vector<std::vector<uint64_t>>;
using signals_table     = std::vector<std::vector<std::string>>;

class DFSTraverser {
 public:
    static auto traverse(const uint64_t           q0,
                         const transitions_table &delta,
                         const signals_table     &phi)
        -> std::tuple<transitions_table, signals_table> {
        DFSTraverser traverser(q0, delta, phi);
        return {traverser.new_delta_, traverser.new_phi_};
    }

 private:
    const transitions_table &delta_;
    const signals_table     &phi_;

    std::vector<bool>        visited_;
    uint64_t                 visited_n = 0;

    transitions_table        new_delta_;
    signals_table            new_phi_;

    std::vector<uint64_t>    new_indices_;

    DFSTraverser(const uint64_t           q0,
                 const transitions_table &delta,
                 const signals_table     &phi)
        : delta_(delta), phi_(phi), visited_(delta.size()),
          new_delta_(delta.size(), std::vector<uint64_t>(delta.at(0).size())),
          new_phi_(phi.size(), std::vector<std::string>(phi.at(0).size())),
          new_indices_(delta_.size()) {
        visit(q0);
        for (uint64_t state_i = 0; state_i < delta_.size(); ++state_i) {
            if (!visited_.at(state_i)) {
                visit(state_i);
            }
        }
    }

    auto visit(const uint64_t current_state) -> void {
        visited_.at(current_state)    = true;
        const auto new_cur_state_name = new_indices_.at(current_state) =
            visited_n++;

        for (uint64_t in_signal_i = 0; in_signal_i < delta_.at(0).size();
             ++in_signal_i) {
            const auto next_state = delta_.at(current_state).at(in_signal_i);
            if (!visited_.at(next_state)) {
                visit(next_state);
            }
            const auto new_next_state_name = new_indices_.at(next_state);

            new_delta_.at(new_cur_state_name).at(in_signal_i) =
                new_next_state_name;

            new_phi_.at(new_cur_state_name).at(in_signal_i) =
                phi_.at(current_state).at(in_signal_i);
        }
    }
};

auto main() -> int {
    uint64_t      n;
    uint64_t      m;
    uint64_t      q0;

    std::istream &in = std::cin;

    in >> n >> m >> q0;

    std::vector<std::vector<uint64_t>> delta(n, std::vector<uint64_t>(m));
    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            uint64_t next_state;
            in >> next_state;
            delta.at(i).at(j) = next_state;
        }
    }

    std::vector<std::vector<std::string>> phi(n, std::vector<std::string>(m));
    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            std::string result;
            in >> result;
            phi.at(i).at(j) = result;
        }
    }

    const auto [new_delta, new_phi] = DFSTraverser::traverse(q0, delta, phi);
    std::cout << n << std::endl << m << std::endl << 0 << std::endl;

    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            std::cout << new_delta.at(i).at(j) << ' ';
        }
        std::cout << std::endl;
    }

    for (uint64_t i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < m; ++j) {
            std::cout << new_phi.at(i).at(j) << ' ';
        }
        std::cout << std::endl;
    }
}
