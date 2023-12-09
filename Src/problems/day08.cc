#include <cstdint>
#include <fstream>
#include <map>
#include <numeric>
#include <string>
#include <utility>

namespace day08 {

struct Input { 
    std::string instructions; 
    std::map<std::string, std::pair<std::string, std::string>> map;
};

static Input get_input()
{
    std::ifstream file("input08.txt");
    Input input;
    std::getline(file, input.instructions);

    std::string line;
    std::getline(file, line); // Skip empty line
    for (std::getline(file, line); !file.eof(); std::getline(file, line))
        input.map[line.substr(0, 3)] = std::make_pair(line.substr(7, 3), line.substr(12, 3));

    return input;
}

std::string part1()
{
    Input input = get_input();
    int step = 0;
    for (std::string pos = "AAA"; pos != "ZZZ"; step++) {
        char c = input.instructions[step % input.instructions.length()];
        pos = (c == 'L' ? input.map[pos].first : input.map[pos].second);
    }

    return std::to_string(step); // 17287
}

static int get_path_length(const Input &input, const std::string &start)
{
    int step = 0;
    for (const std::string *pos = &start; (*pos)[2] != 'Z'; ++step) {
        char c = input.instructions[step % input.instructions.length()];
        pos = c == 'L' ? &input.map.at(*pos).first : &input.map.at(*pos).second;
    }

    return step;
}

std::string part2()
{
    // It turns out that the input is such that we reach ??Z after N, 2N, 3N... steps.
    // We just need to find the least common multiple of the path lengths from ??A to ??Z.
    Input input = get_input();
    uint64_t answer = 1;
    for (const auto &node : input.map) {
        if (node.first[2] == 'A')
            answer = std::lcm(answer, get_path_length(input, node.first));
    }

    return std::to_string(answer); // 18625484023687
}

}
