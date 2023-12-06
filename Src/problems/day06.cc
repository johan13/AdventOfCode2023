#include <cmath>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace day06 {

struct input_t {
    int time;
    int distance;
};

static std::vector<input_t> read_input()
{
    std::vector<input_t> input;
    std::ifstream file("input06.txt");
    std::string times, distances;
    std::getline(file, times);
    std::getline(file, distances);
    std::istringstream time_stream(times);
    std::istringstream dist_stream(distances);

    // Skip "Time:" and "Distance:".
    std::string ignore;
    time_stream >> ignore;
    dist_stream >> ignore;

    while (!time_stream.eof() && !dist_stream.eof()) {
        int time, distance;
        time_stream >> time;
        dist_stream >> distance;
        input.push_back({time, distance});
    }

    return input;
}

std::string part1()
{
    auto input = read_input();

    int answer = 1;
    for (auto [time, distance] : input) {
        int ways_to_win = 0;
        for (int t = 1; t < time; t++) {
            if ((time - t) * t > distance)
                ++ways_to_win;
        }

        answer *= ways_to_win;
    }

    return std::to_string(answer); // 227850
}

std::string part2()
{
    auto input = read_input();
    uint64_t time = 0, distance = 0;
    for (auto [t, d] : input) {
        time = time * std::pow(10, std::ceil(std::log10(t))) + t;
        distance = distance * std::pow(10, std::ceil(std::log10(d))) + d;
    }

    // Bisect the lower half of the parabola
    // (If only there were an anlytical way to solve a 2nd degree equation...)
    uint64_t lower = 0;
    uint64_t upper = time / 2;
    while (upper - lower > 1) {
        uint64_t middle = (lower + upper) / 2;
        if ((time - middle) * middle > distance)
            upper = middle;
        else
            lower = middle;
    }

    uint64_t answer = 2 * (time / 2 - lower) - 1;

    return std::to_string(answer); // 42948149
}

}
