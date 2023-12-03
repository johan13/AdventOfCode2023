#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

struct subset_t {
    int red;
    int green;
    int blue;
};

struct input_t {
    int game;
    std::vector<struct subset_t> subsets;
};

static std::vector<std::string> do_regex_match(const std::string &regex, const std::string &input)
{
    std::smatch match;
    std::regex_match(input, match, std::regex(regex));
    std::vector<std::string> groups;

    for (std::size_t i = 1; i < match.size(); i++) {
        groups.push_back(match[i].str());
    }

    return groups;
}

static std::vector<std::string> split_string(const std::string &input, const char *separator)
{
    const std::size_t separator_length = std::strlen(separator);
    std::vector<std::string> parts;
    for (std::size_t start = 0;;) {
        std::size_t end = input.find(separator, start);
        parts.push_back(input.substr(start, end - start));
        if (end == std::string::npos)
            return parts;

        start = end + separator_length;
    }
}

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

std::string day02p1() {
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


std::string day02p2() {
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
