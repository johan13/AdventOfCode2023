#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace day03 {

struct schematic_t {
    int width;
    int height;
    std::vector<char> data;
};

static struct schematic_t get_input()
{
    struct schematic_t schematic{};
    std::ifstream file("input03.txt");
    std::string line;
    while (std::getline(file, line)) {
        schematic.height++;
        if (schematic.width == 0)
            schematic.width = line.length();
        std::copy(line.begin(), line.end(), std::back_inserter(schematic.data));
    }

    return schematic;
}

static bool is_part_number(const struct schematic_t &schematic, int x0, int x1, int y0)
{
    for (int y = y0 - 1; y <= y0 + 1; y++) {
        if (y >= 0 && y < schematic.height) {
            for (int x = x0 - 1; x <= x1 + 1; x++) {
                char ch = schematic.data[y * schematic.width + x];
                if (x >= 0 && x < schematic.width && ch != '.' && !std::isdigit(ch))
                    return true;
            }
        }
    }

    return false;
}

static int get_number_at(const struct schematic_t &schematic, int x, int y)
{
    const char *row = &schematic.data[y * schematic.width];
    while (x > 0 && std::isdigit(row[x - 1]))
        --x;
    int number = row[x] - '0';
    while (x + 1 < schematic.width && std::isdigit(row[x + 1]))
        number = 10 * number + row[++x] - '0';

    return number;
}

static std::vector<int> get_adjacent_numbers(const struct schematic_t &schematic, int x0, int y0)
{
    std::vector<int> numbers;
    for (int y = std::max(0, y0 - 1); y < std::min(schematic.height, y0 + 2); y++)
    {
        if (std::isdigit(schematic.data[y * schematic.width + x0])) {
            numbers.push_back(get_number_at(schematic, x0, y));
        } else {
            if (x0 > 0 && std::isdigit(schematic.data[y * schematic.width + x0 - 1])) {
                numbers.push_back(get_number_at(schematic, x0 - 1, y));
            }
            if (x0 < schematic.width - 1 && std::isdigit(schematic.data[y * schematic.width + x0 + 1])) {
                numbers.push_back(get_number_at(schematic, x0 + 1, y));
            }
        }
    }

    return numbers;
}

std::string part1() {
    auto schematic = get_input();
    int sum = 0;
    for (int y = 0; y < schematic.height; y++) {
        char *row = &schematic.data[y * schematic.width];
        for (int x = 0; x < schematic.width; x++) {
            if (std::isdigit(row[x])) {
                int start_x = x;
                int number = row[x] - '0';
                while (x + 1 < schematic.width && std::isdigit(row[x + 1]))
                    number = 10 * number + row[++x] - '0';
                if (is_part_number(schematic, start_x, x, y))
                    sum += number;
            }
        }
    }

    return std::to_string(sum); // 525181
}

std::string part2()
{
    auto schematic = get_input();
    int sum = 0;
    for (int y = 0; y < schematic.height; y++) {
        char *row = &schematic.data[y * schematic.width];
        for (int x = 0; x < schematic.width; x++) {
            if (row[x] == '*') {
                auto adjacent = get_adjacent_numbers(schematic, x, y);
                if (adjacent.size() == 2)
                    sum += adjacent[0] * adjacent[1];
            }
        }
    }

    return std::to_string(sum); // 84289137
}

}
