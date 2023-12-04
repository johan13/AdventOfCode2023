#include <algorithm>
#include <cstdint>
#include <fstream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

namespace day04 {

struct input_t {
    std::vector<uint8_t> winning;
    std::vector<uint8_t> ours;
    int count = 1;
};

static std::vector<uint8_t> parse_numbers(std::string_view input)
{
    std::vector<uint8_t> numbers;
    for (std::size_t i = 0; i < input.length(); i += 3)
        numbers.push_back(std::stoi(&input[i]));

    return numbers;
}

static std::vector<struct input_t> read_input()
{
    std::vector<struct input_t> inputs;
    std::ifstream file("input04.txt");
    std::string line;
    while (std::getline(file, line)) {
        int start = line.find(": ");
        int end = line.find(" | ", start + 2);
        inputs.push_back({
            parse_numbers(std::string_view(&line[start + 2], end - start - 2)),
            parse_numbers(std::string_view(&line[end + 3]))
        });
    }

    return inputs;
}

std::string part1()
{
    auto inputs = read_input();
    int sum = 0;
    for (auto &input : inputs) {
        std::sort(input.winning.begin(), input.winning.end());
        std::sort(input.ours.begin(), input.ours.end());
        std::vector<uint8_t> intersection;
        std::set_intersection(input.winning.begin(), input.winning.end(),
                              input.ours.begin(), input.ours.end(),
                              std::back_insert_iterator(intersection));
        if (intersection.size() > 0)
            sum += 1 << (intersection.size() - 1);
    }

    return std::to_string(sum); // 21568
}

std::string part2()
{
    auto inputs = read_input();
    for (std::size_t i = 0; i < inputs.size(); i++) {
        auto &input = inputs[i];
        std::sort(input.winning.begin(), input.winning.end());
        std::sort(input.ours.begin(), input.ours.end());
        std::vector<uint8_t> intersection;
        std::set_intersection(input.winning.begin(), input.winning.end(),
                              input.ours.begin(), input.ours.end(),
                              std::back_insert_iterator(intersection));
        for (std::size_t j = i + 1; j <= i + intersection.size(); j++)
            inputs[j].count += input.count;
    }

    int answer = std::accumulate(inputs.begin(), inputs.end(), 0,
        [](int sum, auto input) { return sum + input.count; });

    return std::to_string(answer); // 11827296
}

}
