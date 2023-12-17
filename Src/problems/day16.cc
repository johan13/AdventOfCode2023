#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace day16 {

typedef std::vector<std::vector<uint8_t>> Map;

// Each map byte contains the type in the lower nibble and the direction that the tile has been
// previously entered in the upper nibble.
const uint8_t EMPTY = 0x00;
const uint8_t F_MIRROR = 0x01;
const uint8_t B_MIRROR = 0x02;
const uint8_t H_SPLIT = 0x03;
const uint8_t V_SPLIT = 0x04;
const uint8_t TYPE_MASK = 0x0f;
const uint8_t RIGHT = 0x10;
const uint8_t UP = 0x20;
const uint8_t LEFT = 0x40;
const uint8_t DOWN = 0x80;
const uint8_t DIR_MASK = 0xf0;

static Map get_input()
{
    std::ifstream file("input16.txt");
    std::string line;
    Map map;
    while (std::getline(file, line)) {
        auto &tiles = map.emplace_back(line.length());
        for (int i = 0; i < (int)line.length(); ++i) {
            if (line[i] == '/')
                tiles[i] = F_MIRROR;
            else if (line[i] == '\\')
                tiles[i] = B_MIRROR;
            else if (line[i] == '-')
                tiles[i] = H_SPLIT;
            else if (line[i] == '|')
                tiles[i] = V_SPLIT;
            else
                tiles[i] = EMPTY;
        }
    }

    return map;
}

static void traverse(Map &map, int x, int y, uint8_t dir)
{
    for (;;) {
        if (dir == RIGHT)
            ++x;
        else if (dir == LEFT)
            --x;
        else if(dir == DOWN)
            ++y;
        else
            --y;

        if (x < 0 || x >= (int)map.front().size() || y < 0 || y >= (int)map.size() || map[y][x] & dir)
            return;

        char type = map[y][x] & TYPE_MASK;
        map[y][x] |= dir;

        if (type == F_MIRROR) {
            if (dir == RIGHT)
                dir = UP;
            else if (dir == UP)
                dir = RIGHT;
            else if (dir == LEFT)
                dir = DOWN;
            else
                dir = LEFT;
        } else if (type == B_MIRROR) {
            if (dir == RIGHT)
                dir = DOWN;
            else if (dir == UP)
                dir = LEFT;
            else if (dir == LEFT)
                dir = UP;
            else
                dir = RIGHT;
        } else if (type == H_SPLIT && (dir == UP || dir == DOWN)) {
            traverse(map, x, y, LEFT);
            traverse(map, x, y, RIGHT);
            return;
        } else if (type == V_SPLIT && (dir == LEFT || dir == RIGHT)) {
            traverse(map, x, y, UP);
            traverse(map, x, y, DOWN);
            return;
        }
    }
}

static int count_energized(const Map &map)
{
    int energized = 0;
    for (auto &row : map) {
        for (char c : row) {
            if (c & DIR_MASK)
                ++energized;
        }
    }

    return energized;
}

std::string part1()
{
    auto input = get_input();
    traverse(input, -1, 0, RIGHT);
    int answer = count_energized(input);

    return std::to_string(answer); // 7979
}

std::string part2()
{
    auto input = get_input();

    int max = 0;
    int height = input.size();
    int width = input.front().size();

    for (int x = 0; x < width; ++x) {
        auto map = Map(input);
        traverse(map, x, -1, DOWN);
        max = std::max(max, count_energized(map));

        map = Map(input);
        traverse(map, x, height, UP);
        max = std::max(max, count_energized(map));
    }

    for (int y = 0; y < height; ++y) {
        auto map = Map(input);
        traverse(map, -1, y, RIGHT);
        max = std::max(max, count_energized(map));

        map = Map(input);
        traverse(map, width, y, LEFT);
        max = std::max(max, count_energized(map));
    }

    return std::to_string(max); // 8437
}

}
