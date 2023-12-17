#include <algorithm>
#include <cstdint>
#include <fstream>
#include <numeric>
#include <span>
#include <string_view>
#include <string>
#include <vector>

namespace day12 {

struct Record {
    std::string wells;
    std::vector<int> groups;
};

static auto get_input()
{
    std::vector<Record> records;
    std::ifstream file("input12.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto &record = records.emplace_back();
        size_t i = line.find(' ');
        record.wells = line.substr(0, i);
        while (i != std::string::npos) {
            record.groups.push_back(std::stoi(&line[i + 1]));
            i = line.find(',', i + 1);
        }
    }

    return records;
}

static int64_t count_arrangements(std::string_view wells, std::span<const int> groups)
{
    if (groups.empty())
        return wells.find('#') == std::string_view::npos ? 1 : 0;
    if (wells.empty())
        return 0;

    // Pick the middle element in 'groups' and find at what index ('i') in 'wells' it can start.
    int group_ix = groups.size() / 2;
    int group = groups[group_ix];
    auto left_groups = groups.subspan(0, group_ix);
    auto right_groups = groups.subspan(group_ix + 1);

    // Find a start_i and end_i that limits the search range so that there are enough '#' and '?'
    // on either side to satisfy the required groups.
    int left_separators = left_groups.size() - 1;
    int left_working = std::ranges::fold_left(left_groups, 0, std::plus());
    int start_i = 0;
    while (left_separators > 0 || left_working > 0) {
        char c = wells[start_i++];
        if (c == '#') {
            --left_working;
        } else if (c == '?') {
            --left_working;
            --left_separators;
        }
    }

    int right_separators = right_groups.size() - 1;
    int right_working = std::ranges::fold_left(right_groups, 0, std::plus());
    int end_i = wells.length();
    while (right_separators > 0 || right_working > 0) {
        char c = wells[--end_i];
        if (c == '#') {
            --right_working;
        } else if (c == '?') {
            --right_working;
            --right_separators;
        }
    }

    end_i -= group;
    int mid_i = (start_i + end_i) / 2;

    int64_t count = 0;
    for (int i = start_i; i <= end_i; ++i) {
        if (i - 1 >= 0 && wells[i - 1] == '#') continue; // No start separator
        if (i + group < (int)wells.length() && wells[i + group] == '#') continue; // No end sep
        if (wells.substr(i, group).find('.') != std::string_view::npos) continue; // Includes working well

        auto left_wells = wells.substr(0, std::max(0, i - 1));
        auto right_wells = wells.substr(std::min(i + group + 1, (int)wells.length()));

        if (i < mid_i) {
            int64_t lhs = count_arrangements(left_wells, left_groups);
            if (lhs > 0) {
                int64_t rhs = count_arrangements(right_wells, right_groups);
                count += lhs * rhs;
            }
        } else {
            int64_t rhs = count_arrangements(right_wells, right_groups);
            if (rhs > 0) {
                int64_t lhs = count_arrangements(left_wells, left_groups);
                count += lhs * rhs;
            }
        }
    }

    return count;
}

std::string part1()
{
    auto records = get_input();
    int answer = std::transform_reduce(
        records.begin(),
        records.end(),
        0,
        std::plus<int>(),
        [](const Record &r) { return count_arrangements(r.wells, std::span(r.groups)); }
    );

    return std::to_string(answer); // 7792
}

std::string part2()
{
    std::vector<Record> records = get_input();

    int64_t answer = 0;
    for (const Record &r : records) {
        std::string extended_wells(r.wells);
        std::vector<int> extended_groups(r.groups);
        for (int i = 0; i < 4; i++) {
            extended_wells += '?';
            extended_wells += r.wells;
            std::copy(r.groups.begin(), r.groups.end(), std::back_inserter(extended_groups));
        }

        answer += count_arrangements(extended_wells, std::span(extended_groups));
    }

    return std::to_string(answer); // 13012052341533
}

}
