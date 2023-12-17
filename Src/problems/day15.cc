#include <cstdint>
#include <fstream>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace day15 {

// The overhead of storing 4000 individual std::string exhausts the RAM. Use callbacks instead.
static void for_each_input(std::function<void(const std::string &)> callback)
{
    std::vector<std::string> strings;
    std::ifstream file("input15.txt");
    std::string str;
    while (std::getline(file, str, ',')) {
        if (str.ends_with("\n"))
            str.erase(str.end() - 1);
        callback(str);
    }
}

static uint8_t calc_hash(const std::string &str)
{
    uint8_t hash = 0;
    for (char c : str)
        hash = (hash + c) * 17;

    return hash;
}

std::string part1()
{
    int sum = 0;
    for_each_input([&](const std::string &str) {
        sum += calc_hash(str);
    });

    return std::to_string(sum); // 521434
}

std::string part2()
{
    std::vector<std::vector<std::pair<std::string, int>>> boxes(256);

    for_each_input([&](const std::string &str) {
        size_t i = str.find('=');
        if (i != std::string::npos) {
            std::string name = str.substr(0, i);
            int focal_length = std::stoi(&str[i + 1]);
            auto &box = boxes[calc_hash(name)];

            auto it = std::ranges::find_if(box, [&](const auto &pair) {
                return pair.first == name;
            });
            if (it == box.end())
                box.push_back({ name, focal_length });
            else
                it->second = focal_length;
        } else {
            std::string name = str.substr(0, str.length() - 1);
            auto &box = boxes[calc_hash(name)];

            auto it = std::find_if(box.begin(), box.end(), [&](const auto &pair) {
                return pair.first == name;
            });
            if (it != box.end())
                box.erase(it);
        }
    });

    int sum = 0;
    for (size_t i = 0; i < boxes.size(); ++i) {
        const auto &box = boxes[i];
        for (size_t j = 0; j < box.size(); ++j)
            sum += (i + 1) * (j + 1) * box[j].second;
    }

    return std::to_string(sum); // 248279
}

}
