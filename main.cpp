#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

std::vector<std::vector<int>> nodes;
std::vector<std::vector<int>> variantsOfGroup;
std::vector<int>              g1;
std::vector<int>              g2;
int                           n;

auto                          makeVariants(std::vector<int> groupNums,
                                           int              universal,
                                           int              first_group_n,
                                           int              second_group_n,
                                           int              current_crewmember_i) -> void {
    if (current_crewmember_i == n) {
        variantsOfGroup.push_back(groupNums);
        g1.push_back(universal);
        g2.push_back(first_group_n);
        return;
    }
    int i = 0;
    for (; i < n; ++i) {
        if (nodes[current_crewmember_i][i] == 1) {
            break;
        }
    }
    if (i == n) {
        groupNums[current_crewmember_i] = 0;
        makeVariants(groupNums,
                     universal + 1,
                     first_group_n,
                     second_group_n,
                     current_crewmember_i + 1);
        return;
    }
    auto group1 = std::vector<int>(groupNums.size());
    auto group2 = std::vector<int>(groupNums.size());
    for (uint64_t k = 0; k < groupNums.size(); ++k) {
        const auto x = groupNums[k];
        group1.at(k) = x;
        group2.at(k) = x;
    }
    group1[current_crewmember_i]    = 1;
    group2[current_crewmember_i]    = 2;
    auto can_add_crewmate_to_first  = true;
    auto can_add_crewmate_to_second = true;
    for (; i < n && (can_add_crewmate_to_first || can_add_crewmate_to_second);
         ++i) {
        if (nodes[current_crewmember_i][i] == 1) {
            if (groupNums[i] == 0) {
                group1[i] = 2;
                group2[i] = 1;
            } else if (groupNums[i] == 1) {
                can_add_crewmate_to_first = false;
            } else if (groupNums[i] == 2) {
                can_add_crewmate_to_second = false;
            }
        }
    }
    if (can_add_crewmate_to_first) {
        makeVariants(group1,
                     universal,
                     first_group_n + 1,
                     second_group_n,
                     current_crewmember_i + 1);
    }
    if (can_add_crewmate_to_second) {
        makeVariants(group2,
                     universal,
                     first_group_n,
                     second_group_n + 1,
                     current_crewmember_i + 1);
    }
}

auto main() -> int {
    std::cin >> n;
    nodes.resize(n);
    std::vector<int> base(n);
    std::string      str;

    getline(std::cin, str);
    for (int i = 0; i < n; ++i) {
        nodes[i].reserve(n);
        getline(std::cin, str);
        for (const auto chr : str) {
            switch (chr) {
                case '+':
                    nodes[i].push_back(1);
                    break;
                case '-':
                    nodes[i].push_back(0);
                    break;
            }
        }
    }

    makeVariants(base, 0, 0, 0, 0);
    if (variantsOfGroup.empty()) {
        std::cout << "No solution" << std::endl;
        return EXIT_SUCCESS;
    }

    for (uint64_t i = 0; i < variantsOfGroup.size(); ++i) {
        g1[i] = n / 2 - g2[i];
    }

    std::vector<std::vector<int>> variants(variantsOfGroup.size());
    for (int i = 0; i < n; ++i) {
        for (uint64_t j = 0; j < variantsOfGroup.size(); ++j) {
            auto x = variantsOfGroup.at(j);
            if (x[i] == 0 && g1[j] > 0) {
                x[i] = 1;
                g1[j]--;
            }
            if (x[i] == 0 && g1[j] == 0) {
                x[i] = 2;
            }
            variants[j].push_back(x[i]);
        }
    }

    auto ans = variants[0];
    for (const auto &x : variantsOfGroup) {
        if (x.size() <= ans.size()) {
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
    }

    for (uint64_t i = 0; i < ans.size(); ++i) {
        const auto x = ans[i];
        if (x == 1) {
            std::cout << i + 1 << ' ';
        }
    }
    std::cout << std::endl;
}
