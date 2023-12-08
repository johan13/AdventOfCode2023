#include <algorithm>
#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace day08 {

typedef std::map<std::string, std::pair<std::string, std::string>> Map;

static std::tuple<std::string, Map> get_input()
{
    std::ifstream file("input08.txt");
    std::string instructions;
    std::getline(file, instructions);

    Map map;
    std::string line;
    std::getline(file, line); // Skip empty line
    for (std::getline(file, line); !file.eof(); std::getline(file, line))
        map[line.substr(0, 3)] = std::make_pair(line.substr(7, 3), line.substr(12, 3));

    return {instructions, map};
}

std::string part1()
{
    auto [instructions, map] = get_input();

    int step = 0;
    for (std::string pos = "AAA"; pos != "ZZZ"; step++) {
        char c = instructions[step % instructions.length()];
        pos = (c == 'L' ? map[pos].first : map[pos].second);
    }

    return std::to_string(step); // 17287
}

struct Periodicity {
    int first;
    int period;
};

static Periodicity find_periodicity(const std::string &instructions, const Map &map, std::string pos)
{
    std::vector<int> step_no_at_z;
    for (int step = 0;;) {
        char c = instructions[step % instructions.length()];
        pos = (c == 'L' ? map.at(pos).first : map.at(pos).second);
        ++step;
        if (pos[2] == 'Z') {
            step_no_at_z.push_back(step);

            // In general it is not enough to have reached Z twice to know the period, but with the
            // provided input (sample and real) this works.
            if (step_no_at_z.size() == 2)
                return {step_no_at_z[0], step_no_at_z[1] - step_no_at_z[0]};
        }
    }
}

std::string part2()
{
    auto [instructions, map] = get_input();

    // For each path, figure out at what step numbers it is at ??Z. (step = first + N * period)
    std::vector<Periodicity> periodicities;
    for (auto node : map) {
        if (node.first[2] == 'A')
            periodicities.push_back(find_periodicity(instructions, map, node.first));
    }

    // Iterate over the paths and find a new first step (called step) and period at which all paths
    // considered so for are at ??Z. When all paths have been considered step is the answer.
    int64_t step = periodicities.front().first;
    int64_t period = periodicities.front().period;
    for (auto it = periodicities.begin() + 1;; ++it) {
        while ((step - it->first) % it->period != 0)
            step += period;
        if (it + 1 == periodicities.end())
            break;
        int64_t next = step + period;
        while ((next - it->first) % it->period)
            next += period;
        period = next - step;
    }

    return std::to_string(step); // 18625484023687
}

}
