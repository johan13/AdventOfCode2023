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

    // Try to bail out early
    int min_wells = 0, max_wells = 0;
    for (char c : wells) {
        if (c == '#') {
            ++min_wells;
            ++max_wells;
        } else if (c == '?') {
            ++max_wells;
        }
    }
    int num_wells = std::ranges::fold_left(groups, 0, std::plus());
    if (num_wells < min_wells || num_wells > max_wells) return 0;

    int64_t count = 0;
    int mid_group_ix = groups.size() / 2;
    int mid_group_sz = groups[mid_group_ix];
    for (int i = 0; i <= (int)wells.length() - mid_group_sz; ++i) {
        if (i - 1 >= 0 && wells[i - 1] == '#') continue; // No start separator
        if (i + mid_group_sz < (int)wells.length() && wells[i + mid_group_sz] == '#') continue; // No end sep
        if (wells.substr(i, mid_group_sz).find('.') != std::string_view::npos) continue; // Includes working well

        auto left_wells = wells.substr(0, std::max(0, i - 1));
        auto right_wells = wells.substr(std::min(i + mid_group_sz + 1, (int)wells.length()));
        auto left_groups = groups.subspan(0, mid_group_ix);
        auto right_groups = groups.subspan(mid_group_ix + 1);

        int64_t lhs = count_arrangements(left_wells, left_groups);
        int64_t rhs = count_arrangements(right_wells, right_groups);
        count += lhs * rhs;
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
