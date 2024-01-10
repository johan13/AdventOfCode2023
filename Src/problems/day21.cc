#include <cstdint>
#include <fstream>
#include <set>
#include <string>
#include <vector>

namespace day21 {

typedef std::vector<std::string> Map;

struct Pos {
    uint8_t x;
    uint8_t y;

    const bool operator<(const Pos &other) const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        return y < other.y;
    }
};

static Map get_input()
{
    Map map;
    std::ifstream file("input21.txt");
    std::string line;
    while (std::getline(file, line))
        map.push_back(line);

    return map;
}

std::string part1()
{
    Map map = get_input();
    uint8_t width = map.front().length();
    uint8_t height = map.size();

    std::set<Pos> positions; // TODO: Use vector to save memory? Using 140 kB now.
    for (uint8_t y = 0; y < height; ++y) {
        for (uint8_t x = 0; x < width; ++x) {
            if (map[y][x] == 'S') {
                positions.insert({ x, y });
                map[y][x] = '.';
            }
        }
    }

    for (int step = 0; step < 64; ++step) {
        std::vector<Pos> old_positions(positions.begin(), positions.end());
        positions.clear();
        for (auto [x, y] : old_positions) {
            if (x > 0 && map[y][x - 1] == '.') positions.insert({ (uint8_t)(x - 1), y });
            if (x < width - 1 && map[y][x + 1] == '.') positions.insert({ (uint8_t)(x + 1), y });
            if (y > 0 && map[y - 1][x] == '.') positions.insert({ x, (uint8_t)(y - 1) });
            if (y < height - 1 && map[y + 1][x] == '.') positions.insert({ x, (uint8_t)(y + 1) });
        }
    }

    return std::to_string(positions.size()); // 3658
}

std::string part2()
{
    return "TODO";
}

}
