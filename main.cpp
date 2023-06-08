#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

enum class CrewmemberStatus { FREE, FIRST_GROUP, SECOND_GROUP };

std::vector<std::vector<bool>>             nodes;
std::vector<std::vector<CrewmemberStatus>> group_variants;
std::vector<uint64_t>                      people_needed_for_first_group;
std::vector<uint64_t>                      first_group_sizes;
uint64_t                                   n;

auto make_variants(std::vector<CrewmemberStatus> group,
                   uint64_t                      first_group_n,
                   uint64_t                      current_crewmember_i) -> void {
    if (current_crewmember_i == n) {
        group_variants.push_back(group);
        first_group_sizes.push_back(first_group_n);
        return;
    }
    uint64_t i = 0;
    for (; i < n; ++i) {
        if (nodes[current_crewmember_i][i] == 1) {
            break;
        }
    }
    if (i == n) {
        group[current_crewmember_i] = CrewmemberStatus::FREE;
        make_variants(group, first_group_n, current_crewmember_i + 1);
        return;
    }
    auto group1 = std::vector<CrewmemberStatus>(group.size());
    auto group2 = std::vector<CrewmemberStatus>(group.size());
    for (uint64_t k = 0; k < group.size(); ++k) {
        const auto x = group[k];
        group1.at(k) = x;
        group2.at(k) = x;
    }
    group1[current_crewmember_i]    = CrewmemberStatus::FIRST_GROUP;
    group2[current_crewmember_i]    = CrewmemberStatus::SECOND_GROUP;
    auto can_add_crewmate_to_first  = true;
    auto can_add_crewmate_to_second = true;
    for (; i < n && (can_add_crewmate_to_first || can_add_crewmate_to_second);
         ++i) {
        if (nodes[current_crewmember_i][i] == 1) {
            if (group[i] == CrewmemberStatus::FREE) {
                group1[i] = CrewmemberStatus::SECOND_GROUP;
                group2[i] = CrewmemberStatus::FIRST_GROUP;
            } else if (group[i] == CrewmemberStatus::FIRST_GROUP) {
                can_add_crewmate_to_first = false;
            } else if (group[i] == CrewmemberStatus::SECOND_GROUP) {
                can_add_crewmate_to_second = false;
            }
        }
    }
    if (can_add_crewmate_to_first) {
        make_variants(group1, first_group_n + 1, current_crewmember_i + 1);
    }
    if (can_add_crewmate_to_second) {
        make_variants(group2, first_group_n, current_crewmember_i + 1);
    }
}

auto main() -> int {
    std::cin >> n;
    nodes.resize(n);
    std::vector<CrewmemberStatus> base(n, CrewmemberStatus::FREE);
    std::string                   str;

    getchar();
    for (uint64_t i = 0; i < n; ++i) {
        nodes[i].reserve(n);
        int chr;
        while ((chr = getchar()) != EOF || chr != '\n') {
            if (chr == '+') {
                nodes[i].push_back(true);
            } else if (chr == '-') {
                nodes[i].push_back(false);
            } else if (chr == ' ') {
                continue;
            } else {
                break;
            }
        }
    }

    make_variants(base, 0, 0);
    if (group_variants.empty()) {
        std::cout << "No solution" << std::endl;
        return EXIT_SUCCESS;
    }

    people_needed_for_first_group.resize(first_group_sizes.size(), 0);
    const auto average_group_size = n >> 1;
    for (uint64_t i = 0; i < group_variants.size(); ++i) {
        if (average_group_size > first_group_sizes[i]) {
            people_needed_for_first_group[i] =
                average_group_size - first_group_sizes[i];
        } else {
            people_needed_for_first_group[i] = 0;
        }
    }

    std::vector<std::vector<CrewmemberStatus>> variants(group_variants.size());
    for (uint64_t group_i = 0; group_i < group_variants.size(); ++group_i) {
        for (uint64_t crewmate_i = 0; crewmate_i < n; ++crewmate_i) {
            auto current_group = group_variants.at(group_i);
            if (current_group[crewmate_i] == CrewmemberStatus::FREE &&
                people_needed_for_first_group[group_i] > 0) {
                current_group[crewmate_i] = CrewmemberStatus::FIRST_GROUP;
                --people_needed_for_first_group[group_i];
            }
            if (current_group[crewmate_i] == CrewmemberStatus::FREE &&
                people_needed_for_first_group[group_i] == 0) {
                current_group[crewmate_i] = CrewmemberStatus::SECOND_GROUP;
            }
            variants[group_i].push_back(current_group[crewmate_i]);
        }
    }

    auto ans = variants[0];
    for (const auto &x : group_variants) {
        if (x.size() > ans.size()) {
            continue;
        }
        for (uint64_t i = 0; i < x.size(); ++i) {
            if (ans[i] > x[i]) {
                ans = x;
                break;
            }
            if (ans[i] < x[i]) {
                break;
            }
        }
    }

    for (uint64_t i = 0; i < ans.size(); ++i) {
        const auto x = ans[i];
        if (x == CrewmemberStatus::FIRST_GROUP) {
            std::cout << i + 1 << ' ';
        }
    }
    std::cout << std::endl;
}
