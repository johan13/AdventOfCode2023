#include <algorithm>
#include <cstdint>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace day18 {

struct Input {
    char dir; // R, U, L or D
    int distance;
};

static auto get_input1()
{
    std::vector<Input> inputs;
    std::ifstream file("input18.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto &input = inputs.emplace_back();
        input.dir = line[0];
        input.distance = std::stoi(&line[2]);
    }

    return inputs;
}

static auto get_input2()
{
    std::vector<Input> inputs;
    std::ifstream file("input18.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto &input = inputs.emplace_back();
        uint32_t color = std::stoi(&line[line.find('#') + 1], nullptr, 16);
        input.dir = "RDLU"[color & 3];
        input.distance = color >> 4;
    }

    return inputs;
}

static std::string solution(const std::vector<Input> &input)
{
    // Find what values x and y can take.
    std::set<int> xvalset{0}, yvalset{0};
    int x = 0, y = 0;
    for (auto [direction, distance] : input) {
        if (direction == 'R') x += distance;
        if (direction == 'U') y -= distance;
        if (direction == 'L') x -= distance;
        if (direction == 'D') y += distance;
        xvalset.insert(x);
        yvalset.insert(y);
    }
    std::vector<int> xvals(xvalset.begin(), xvalset.end());
    std::vector<int> yvals(yvalset.begin(), yvalset.end());

    // We need to be smart about memory usage here. Make a new grid with one cell per xval and
    // interval between adjacent xvals along the x axis, and similarly along the y axis.
    // Use one bit per grid cell. 1 = '#', 0 = '.'. (The map vector uses about 57 kB.)

    int width = xvals.size() * 2 - 1;
    int height = yvals.size() * 2 - 1;
    int bytes_per_row = (width + 7) / 8;
    std::vector<uint8_t> map(height * bytes_per_row);

    auto get = [&](int x, int y) -> bool {
        return (map[y * bytes_per_row + (x >> 3)] & (1 << (x & 7))) != 0;
    };
    auto set = [&](int x, int y) {
        map[y * bytes_per_row + (x >> 3)] |= 1 << (x & 7);
    };
    auto get_x = [&](int raw_x) {
        return 2 * (std::find(xvals.begin(), xvals.end(), raw_x) - xvals.begin());
    };
    auto get_y = [&](int raw_y) {
        return 2 * (std::find(yvals.begin(), yvals.end(), raw_y) - yvals.begin());
    };

    int raw_x = 0;
    int raw_y = 0;
    x = get_x(raw_x);
    y = get_y(raw_y);
    for (auto [direction, distance] : input) {
        if (direction == 'R') {
            raw_x += distance;
            int new_x = get_x(raw_x);
            while (x != new_x) set(++x, y);
        } else if (direction == 'U') {
            raw_y -= distance;
            int new_y = get_y(raw_y);
            while (y != new_y) set(x, --y);
        } else if (direction == 'L') {
            raw_x -= distance;
            int new_x = get_x(raw_x);
            while (x != new_x) set(--x, y);
        } else if (direction == 'D') {
            raw_y += distance;
            int new_y = get_y(raw_y);
            while (y != new_y) set(x, ++y);
        }
    }

    // Find an x,y coordinate inside the border.
    y = 1;
    x = 0;
    while (!get(x++, y));

    // Fill the area.
    std::set<std::pair<int, int>> queue{{x, y}};
    while (!queue.empty()) {
        auto [x, y] = *queue.begin();
        queue.erase(queue.begin());
        set(x, y);
        if (x < width - 1 && !get(x + 1, y)) queue.insert({ x + 1, y });
        if (y > 0 && !get(x, y - 1)) queue.insert({ x, y - 1 });
        if (x > 0 && !get(x - 1, y)) queue.insert({ x - 1, y });
        if (y < height - 1 && !get(x, y + 1)) queue.insert({ x, y + 1 });
    }

    // Sum up the dug out area.
    int64_t answer = 0;
    for (int y = 0; y < height; ++y) {
        int64_t y_mult = y % 2 == 0 ? 1 : yvals[y / 2 + 1] - yvals[y / 2] - 1;
        for (int x = 0; x < width; ++x) {
            if (get(x, y)) {
                int64_t x_mult = (x % 2 == 0 ? 1 : xvals[x / 2 + 1] - xvals[x / 2] - 1);
                answer += y_mult * x_mult;
            }
        }
    }

    return std::to_string(answer);
}

std::string part1()
{
    return solution(get_input1()); // 108909
}

std::string part2()
{
    return solution(get_input2()); // 133125706867777
}

}
