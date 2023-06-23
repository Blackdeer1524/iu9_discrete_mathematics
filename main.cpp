#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SetItem {
 public:
    SetItem() : parent(this) {
    }

    auto unite(SetItem *other) -> void {
        link(find(), other->find());
    }

    auto find() -> SetItem * {
        if (this != parent) {
            parent = parent->find();
        }
        return parent;
    }

    uint64_t rank{};
    SetItem *parent;

 private:
    static auto link(SetItem *x, SetItem *y) -> void {
        if (x->rank > y->rank) {
            y->parent = x;
            return;
        }
        x->parent = y;
        if (x->rank == y->rank) {
            ++y->rank;
        }
    }
};

struct State {
    explicit State(uint64_t transitions_size)
        : equivalence_class(nullptr), transitions(transitions_size) {
    }

    SetItem              *equivalence_class;
    std::vector<uint64_t> transitions;
};

using transitions_table = std::vector<State>;
using signals_table     = std::vector<std::vector<std::string>>;

struct Automaton {
    uint64_t          q0;
    transitions_table delta;
    signals_table     phi;
    uint64_t          states_count;
    uint64_t          alphabet_size;

    Automaton(const Automaton &)                     = delete;
    Automaton(Automaton &&)                          = default;
    auto operator=(const Automaton &) -> Automaton & = delete;
    auto operator=(Automaton &&) -> Automaton      & = default;
};

auto split1(const Automaton &A)
    -> std::tuple<uint64_t, std::vector<SetItem *>> {
    uint64_t m = A.states_count;
    for (const auto &state : A.delta) {
        state.equivalence_class->parent = state.equivalence_class;
        state.equivalence_class->rank   = 0;
    }

    for (uint64_t q1_index = 0; q1_index < A.states_count; ++q1_index) {
        const auto &q1 = A.delta.at(q1_index);
        for (uint64_t q2_index = 0; q2_index < A.states_count; ++q2_index) {
            const auto &q2 = A.delta.at(q2_index);
            if (q1.equivalence_class->find() == q2.equivalence_class->find()) {
                continue;
            }
            bool eq = true;
            for (uint64_t in_signal_i = 0; in_signal_i < A.alphabet_size;
                 ++in_signal_i) {
                if (A.phi.at(q1_index).at(in_signal_i) !=
                    A.phi.at(q2_index).at(in_signal_i)) {
                    eq = false;
                    break;
                }
            }
            if (eq) {
                q1.equivalence_class->unite(q2.equivalence_class);
                --m;
            }
        }
    }
    std::vector<SetItem *> state2equivalence_class(A.states_count);
    for (uint64_t i = 0; i < A.states_count; ++i) {
        state2equivalence_class.at(i) = A.delta.at(i).equivalence_class->find();
    }

    return {m, state2equivalence_class};
}

// #include <gtest/gtest.h>
//
// TEST(Split1, TwoClasses) {
//     uint64_t                           n = 3;
//     transitions_table                  delta{n, State(0)};
//     signals_table                      phi{n};
//
//     std::vector<std::vector<uint64_t>> transitions{
//         {0, 2},
//         {1, 2},
//         {2, 2}
//     };
//
//     std::vector<std::vector<std::string>> outputs{
//         {"0", "1"},
//         {"0", "1"},
//         {"2", "2"}
//     };
//
//     for (uint64_t i = 0; i < n; ++i) {
//         delta.at(i).transitions       = transitions.at(i);
//         delta.at(i).equivalence_class = new SetItem;
//
//         phi.at(i)                     = outputs.at(i);
//     }
//
//     Automaton a{0, delta, phi, n, 2};
//     const auto [eq_classes_count, state2eq_class] = split1(a);
//     ASSERT_EQ(eq_classes_count, 2);
//     ASSERT_EQ(state2eq_class.at(0)->find(), state2eq_class.at(1)->find());
//     ASSERT_NE(state2eq_class.at(0)->find(), state2eq_class.at(2)->find());
//     ASSERT_NE(state2eq_class.at(1)->find(), state2eq_class.at(2)->find());
// }

auto split(const Automaton &A, std::vector<SetItem *> &state2eq_class)
    -> std::tuple<uint64_t, std::vector<SetItem *>> {
    uint64_t eq_classes_count = A.states_count;
    for (const auto &state : A.delta) {
        state.equivalence_class->parent = state.equivalence_class;
        state.equivalence_class->rank   = 0;
    }
    for (uint64_t q0_index = 0; q0_index < A.states_count; ++q0_index) {
        const auto &q0 = A.delta.at(q0_index);
        for (uint64_t q1_index = q0_index + 1; q1_index < A.states_count;
             ++q1_index) {
            const auto &q1 = A.delta.at(q1_index);
            if (state2eq_class.at(q0_index) != state2eq_class.at(q1_index) ||
                q0.equivalence_class->find() == q1.equivalence_class->find()) {
                continue;
            }
            bool eq = true;
            for (uint64_t in_signal_i = 0; in_signal_i < A.alphabet_size;
                 ++in_signal_i) {
                const auto state_after_q0 =
                    A.delta.at(q0_index).transitions.at(in_signal_i);
                const auto state_after_q1 =
                    A.delta.at(q1_index).transitions.at(in_signal_i);
                if (state2eq_class.at(state_after_q0) !=
                    state2eq_class.at(state_after_q1)) {
                    eq = false;
                    break;
                }
            }
            if (eq) {
                q0.equivalence_class->unite(q1.equivalence_class);
                --eq_classes_count;
            }
        }
    }

    std::vector<SetItem *> k_plus_one_equivalent_clases(state2eq_class.size());
    for (uint64_t i = 0; i < A.states_count; ++i) {
        k_plus_one_equivalent_clases.at(i) =
            A.delta.at(i).equivalence_class->find();
    }
    return {eq_classes_count, k_plus_one_equivalent_clases};
}

auto aufenkamp_hohn(const Automaton &A)
    -> std::tuple<Automaton, std::vector<SetItem *>> {
    auto [k_equivalent_count, state_i2k_eq_class] = split1(A);
    for (;;) {
        auto [k_plus_one_equivalent_count, state_i2k_plus_one_eq_class] =
            split(A, state_i2k_eq_class);
        if (k_plus_one_equivalent_count == k_equivalent_count) {
            break;
        }
        state_i2k_eq_class = std::move(state_i2k_plus_one_eq_class);
        k_equivalent_count = k_plus_one_equivalent_count;
    }

    std::vector<SetItem *> set_items_pool(k_equivalent_count);
    transitions_table      new_delta(k_equivalent_count, State(0));
    signals_table          new_phi(k_equivalent_count);
    for (uint64_t i = 0; i < k_equivalent_count; ++i) {
        new_delta.at(i).equivalence_class = set_items_pool.at(i) = new SetItem;
        new_delta.at(i).transitions.resize(A.alphabet_size);
        new_phi.at(i).resize(A.alphabet_size);
    }

    std::unordered_map<const SetItem *, uint64_t> eq_class2name;
    std::unordered_set<const SetItem *>           visited;
    uint64_t                                      defined_new_states = 0;
    for (uint64_t state_i = 0; state_i < A.states_count; ++state_i) {
        const auto *cur_equivalence_class = state_i2k_eq_class.at(state_i);
        if (visited.find(cur_equivalence_class) != visited.end()) {
            continue;
        }
        visited.emplace(cur_equivalence_class);

        uint64_t cur_eq_class_name;
        if (const auto found = eq_class2name.find(cur_equivalence_class);
            found == eq_class2name.end()) {
            eq_class2name.insert({cur_equivalence_class, defined_new_states});
            cur_eq_class_name = defined_new_states;
            ++defined_new_states;
        } else {
            cur_eq_class_name = found->second;
        }

        for (uint64_t signal_i = 0; signal_i < A.alphabet_size; ++signal_i) {
            const auto next_state_i =
                A.delta.at(state_i).transitions.at(signal_i);

            const auto *next_equivalence_class =
                state_i2k_eq_class.at(next_state_i);
            uint64_t next_eq_class_name;
            if (const auto found = eq_class2name.find(next_equivalence_class);
                found == eq_class2name.end()) {
                eq_class2name.insert(
                    {next_equivalence_class, defined_new_states});
                next_eq_class_name = defined_new_states;
                ++defined_new_states;
            } else {
                next_eq_class_name = found->second;
            }

            new_delta.at(cur_eq_class_name).transitions.at(signal_i) =
                next_eq_class_name;
            new_phi.at(cur_eq_class_name).at(signal_i) =
                A.phi.at(state_i).at(signal_i);
        }
    }
    const auto    *q0_eq_class = state_i2k_eq_class.at(A.q0);
    const uint64_t new_q0      = eq_class2name.at(q0_eq_class);

    Automaton      minimized{
        new_q0, new_delta, new_phi, k_equivalent_count, A.alphabet_size};

    return {std::move(minimized), set_items_pool};
}

// TEST(AufenkampHohn, basic) {
//     uint64_t                           n = 3;
//     transitions_table                  delta{n, State(0)};
//     signals_table                      phi{n};
//
//     std::vector<std::vector<uint64_t>> transitions{
//         {0, 2},
//         {1, 2},
//         {2, 2}
//     };
//
//     std::vector<std::vector<std::string>> outputs{
//         {"0", "1"},
//         {"0", "1"},
//         {"2", "2"}
//     };
//
//     for (uint64_t i = 0; i < n; ++i) {
//         delta.at(i).transitions       = transitions.at(i);
//         delta.at(i).equivalence_class = new SetItem;
//
//         phi.at(i)                     = outputs.at(i);
//     }
//
//     Automaton  a{0, delta, phi, n, 2};
//     const auto minimized_a = aufenkamp_hohn(a);
//     ASSERT_EQ(minimized_a.q0, 0);
//     ASSERT_EQ(minimized_a.states_count, 2);
//
//     ASSERT_EQ(minimized_a.delta.at(0).transitions.at(0), 0);
//     ASSERT_EQ(minimized_a.delta.at(0).transitions.at(1), 1);
//
//     ASSERT_EQ(minimized_a.delta.at(1).transitions.at(0), 1);
//     ASSERT_EQ(minimized_a.delta.at(1).transitions.at(1), 1);
// }

class DFSTraverser {
 public:
    static auto traverse(const Automaton &A) -> Automaton {
        DFSTraverser traverser(A);
        return std::move(traverser.canonical_);
    }

 private:
    std::vector<bool>     visited_;
    uint64_t              visited_n = 0;

    const Automaton      &A_;
    Automaton             canonical_;

    std::vector<uint64_t> new_indices_;

    explicit DFSTraverser(const Automaton &A)
        : visited_(A.states_count), A_(A),
          canonical_({A.q0, {}, {}, A.states_count, A.alphabet_size}),
          new_indices_(A.states_count) {
        canonical_.delta.resize(A.states_count, State(A.alphabet_size));
        canonical_.phi.resize(A.states_count,
                              std::vector<std::string>(A.alphabet_size));

        visit(A.q0);
        for (uint64_t state_i = 0; state_i < canonical_.states_count;
             ++state_i) {
            if (!visited_.at(state_i)) {
                visit(state_i);
            }
        }
    }

    auto visit(const uint64_t current_state) -> void {
        visited_.at(current_state)    = true;
        const auto new_cur_state_name = new_indices_.at(current_state) =
            visited_n++;

        for (uint64_t in_signal_i = 0; in_signal_i < canonical_.alphabet_size;
             ++in_signal_i) {
            const auto next_state =
                A_.delta.at(current_state).transitions.at(in_signal_i);
            if (!visited_.at(next_state)) {
                visit(next_state);
            }
            const auto new_next_state_name = new_indices_.at(next_state);

            canonical_.delta.at(new_cur_state_name)
                .transitions.at(in_signal_i) = new_next_state_name;

            canonical_.phi.at(new_cur_state_name).at(in_signal_i) =
                A_.phi.at(current_state).at(in_signal_i);
        }
    }
};

auto read_automaton(std::istream &in)
    -> std::tuple<Automaton, std::vector<SetItem *>> {
    uint64_t n;
    uint64_t m;
    uint64_t q0;

    in >> n >> m >> q0;

    transitions_table      delta(n, State(m));
    std::vector<SetItem *> set_items_pool(n);
    for (uint64_t i = 0; i < n; ++i) {
        delta.at(i).equivalence_class = set_items_pool.at(i) = new SetItem;
        for (uint64_t j = 0; j < m; ++j) {
            uint64_t next_state;
            in >> next_state;
            delta.at(i).transitions.at(j) = next_state;
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

    Automaton a{q0, delta, phi, n, m};
    return {std::move(a), set_items_pool};
}

auto main() -> int {
    std::istream &in                           = std::cin;

    const auto [first, first_set_items_pool]   = read_automaton(in);
    const auto [second, second_set_items_pool] = read_automaton(in);

    const auto [minimized_first, minimized_first_set_items_pool] =
        aufenkamp_hohn(first);
    const auto [minimized_second, minimized_second_set_items_pool] =
        aufenkamp_hohn(second);

    if (minimized_first.states_count != minimized_second.states_count) {
        std::cout << "NOT EQUAL" << std::endl;
        return EXIT_SUCCESS;
    }

    // В canonical_ ВСЕ УКАЗАТЕЛИ В `delta` будут nullptr
    const auto canonical_first  = DFSTraverser::traverse(minimized_first);
    const auto canonical_second = DFSTraverser::traverse(minimized_second);

    for (uint64_t i = 0; i < canonical_first.states_count; ++i) {
        for (uint64_t j = 0; j < canonical_first.alphabet_size; ++j) {
            if (canonical_first.delta.at(i).transitions.at(j) !=
                    canonical_second.delta.at(i).transitions.at(j) ||
                canonical_first.phi.at(i).at(j) !=
                    canonical_second.phi.at(i).at(j)) {
                std::cout << "NOT EQUAL" << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }
    std::cout << "EQUAL" << std::endl;
}
