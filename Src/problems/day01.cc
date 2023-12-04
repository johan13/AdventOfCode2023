#include <fstream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

namespace day01 {

static std::vector<std::tuple<const char *, int>> textual{
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
};

static int parse_line(const std::string &line, bool look_for_text)
{
    size_t first_pos = line.find_first_of("1234567890");
    int first_number = line[first_pos] - '0';
    size_t last_pos = line.find_last_of("1234567890");
    int last_number = line[last_pos] - '0';

    if (look_for_text)
    {
        for (auto [str, num] : textual)
        {
            size_t i = line.find(str);
            if (i != line.npos && i < first_pos)
            {
                first_number = num;
                first_pos = i;
            }

            i = line.rfind(str);
            if (i != line.npos && i > last_pos)
            {
                last_number = num;
                last_pos = i;
            }
        }
    }

    return first_number * 10 + last_number;
}

static std::string solution(bool look_for_text)
{
    std::ifstream file("input01.txt");
    std::vector<std::string> input;
    while (std::getline(file, input.emplace_back()))
        ;

    int answer = std::accumulate(input.begin(), input.end() - 1, 0,
        [look_for_text](int sum, const std::string &str)
        {
            return sum + parse_line(str, look_for_text);
        });

    return std::to_string(answer);
}

std::string part1()
{
    return solution(false); // 54605
}

std::string part2()
{
    return solution(true); // 55429
}

}
