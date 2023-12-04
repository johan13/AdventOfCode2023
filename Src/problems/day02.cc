#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>
#include "common.h"

namespace day02 {

struct subset_t {
    int red;
    int green;
    int blue;
};

struct input_t {
    int game;
    std::vector<struct subset_t> subsets;
};

static input_t parse_line(const std::string &line) {
    input_t input;

    auto captures = do_regex_match("Game (\\d*): (.*)", line);
    input.game = std::atoi(captures[0].c_str());

    for (const std::string &subset_str : split_string(captures[1], "; ")) {
        auto &subset = input.subsets.emplace_back();
        for (const std::string &cube : split_string(subset_str, ", ")) {
            int count = std::atoi(cube.c_str());
            if (cube.find("red") != std::string::npos)
                subset.red = count;
            else if (cube.find("green") != std::string::npos)
                subset.green = count;
            else
                subset.blue = count;
        }
    }

    return input;
}

static std::vector<struct input_t> read_input() {
    std::ifstream file("input02.txt");
    std::vector<struct input_t> input;
    std::string line;
    while (std::getline(file, line)) {
        input.push_back(parse_line(line));
    }

    return input;
}

std::string part1() {
    auto input = read_input();

    int answer = std::accumulate(
        input.begin(), input.end(), 0, [](int sum, const struct input_t &game) {
            bool playable =
                std::all_of(game.subsets.begin(), game.subsets.end(),
                            [](const struct subset_t &subset) {
                                return subset.red <= 12 && subset.green <= 13 &&
                                       subset.blue <= 14;
                            });
            return sum + (playable ? game.game : 0);
        });

    return std::to_string(answer); // 2348
}


std::string part2() {
    auto input = read_input();

    int answer = std::accumulate(
        input.begin(), input.end(), 0, [](int sum, const struct input_t &game) {
            int min_red = 0, min_green = 0, min_blue = 0;
            for (auto &subset : game.subsets) {
                min_red = std::max(min_red, subset.red);
                min_green = std::max(min_green, subset.green);
                min_blue = std::max(min_blue, subset.blue);
            }
            return sum + min_red * min_green * min_blue;
        });

    return std::to_string(answer); // 76008
}

}
