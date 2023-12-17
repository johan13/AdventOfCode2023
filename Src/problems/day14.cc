#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

namespace day14 {

typedef std::vector<std::string> Map;

static Map get_input()
{
    Map map;
    std::ifstream file("input14.txt");
    std::string line;
    while (std::getline(file, line))
        map.push_back(line);

    return map;
}

static void roll_north(Map &map)
{
    for (int x = 0; x < (int)map.front().length(); ++x) {
        int roll_to = 0;
        for (int y = 0; y < (int)map.size(); ++y) {
            if (map[y][x] == 'O') {
                if (roll_to != y) {
                    map[roll_to][x] = 'O';
                    map[y][x] = '.';
                }
                ++roll_to;
            } else if (map[y][x] == '#') {
                roll_to = y + 1;
            }
        }
    }
}

static void roll_south(Map &map)
{
    for (int x = 0; x < (int)map.front().length(); ++x) {
        int roll_to = map.size() - 1;
        for (int y = map.size() - 1; y >= 0; --y) {
            if (map[y][x] == 'O') {
                if (roll_to != y) {
                    map[roll_to][x] = 'O';
                    map[y][x] = '.';
                }
                --roll_to;
            } else if (map[y][x] == '#') {
                roll_to = y - 1;
            }
        }
    }
}

static void roll_west(Map &map)
{
    for (int y = 0; y < (int)map.size(); ++y) {
        int roll_to = 0;
        for (int x = 0; x < (int)map.front().length(); ++x) {
            if (map[y][x] == 'O') {
                if (roll_to != x) {
                    map[y][roll_to] = 'O';
                    map[y][x] = '.';
                }
                ++roll_to;
            } else if (map[y][x] == '#') {
                roll_to = x + 1;
            }
        }
    }
}

static void roll_east(Map &map)
{
    for (int y = 0; y < (int)map.size(); ++y) {
        int roll_to = map.front().length() - 1;
        for (int x = map.front().length() - 1; x >= 0; --x) {
            if (map[y][x] == 'O') {
                if (roll_to != x) {
                    map[y][roll_to] = 'O';
                    map[y][x] = '.';
                }
                --roll_to;
            } else if (map[y][x] == '#') {
                roll_to = x - 1;
            }
        }
    }
}

static void do_cycle(Map &map)
{
    roll_north(map);
    roll_west(map);
    roll_south(map);
    roll_east(map);
}

static int calc_load(const Map &map)
{
    int sum = 0;
    for (int y = 0; y < (int)map.size(); ++y) {
        int factor = map.size() - y;
        for (int x = 0; x < (int)map.front().length(); ++x) {
            if (map[y][x] == 'O')
                sum += factor;
        }
    }

    return sum;
}

std::string part1()
{
    Map map = get_input();
    roll_north(map);
    int answer = calc_load(map);

    return std::to_string(answer); // 110821
}

std::string part2()
{
    const int num_cycles = 1000000000;
    const int repetition_length = 20; // The real period turns out to be 11.

    Map map = get_input();
    std::vector<int> loads;
    for (int cycle = 0;; cycle++) {
        do_cycle(map);
        loads.push_back(calc_load(map));

        // Look for a repeating sequence.
        if (loads.size() > repetition_length) {
            auto repetition_start = std::find_end(loads.begin(), loads.end() - 1, loads.end() - repetition_length, loads.end());
            if (repetition_start != loads.end() - 1) {
                int offs = (num_cycles - 1 - (repetition_start - loads.begin()))
                    % (loads.end() - repetition_length - repetition_start)
                    + (repetition_start - loads.begin());
                return std::to_string(loads[offs]); // 83516
            }
        }
    }
}

}
