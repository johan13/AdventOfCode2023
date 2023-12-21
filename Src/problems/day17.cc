#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

namespace day17 {

typedef std::vector<std::vector<uint8_t>> Map;

enum struct Dir : uint8_t { Right, Up, Left, Down };

struct Entry {
    uint8_t x;
    uint8_t y;
    Dir dir;
    uint8_t num_steps;
    uint16_t acc_heat_loss;
    uint16_t min_remaining;
};

static uint16_t manhattan(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

static bool heap_compare(const Entry &lhs, const Entry &rhs)
{
    // Use > instead of < to make a min heap instead of a max heap.
    return lhs.acc_heat_loss + lhs.min_remaining > rhs.acc_heat_loss + rhs.min_remaining;
}

static Map get_input()
{
    std::ifstream file("input17.txt");
    std::string line;
    Map map;
    while (std::getline(file, line)) {
        auto &row = map.emplace_back(line.length());
        for (auto [i, c] : std::views::enumerate(line))
            row[i] = c - '0';
    }

    return map;
}

static void add_to_queue(std::vector<Entry> &queue, const Entry &entry)
{
    auto it = std::find_if(queue.begin(), queue.end(), [&](const Entry &other) {
        return
            other.x == entry.x &&
            other.y == entry.y &&
            other.dir == entry.dir &&
            other.num_steps == entry.num_steps && // TODO: <= for part 1
            other.acc_heat_loss + other.min_remaining <= entry.acc_heat_loss + entry.min_remaining;
    });
    if (it != queue.end())
        return;
    
    queue.push_back(std::move(entry));
    std::push_heap(queue.begin(), queue.end(), heap_compare);
}

std::string part1()
{
    Map map = get_input();
    auto max_dist = manhattan(0, 0, map.size(), map.front().size());
    std::vector<Entry> queue({
        { 0, 0, Dir::Right, 0, 0, max_dist },
        { 0, 0, Dir::Down, 0, 0, max_dist },
    });

    for (;;) {
        auto node = queue[0];
        std::pop_heap(queue.begin(), queue.end(), heap_compare);
        queue.pop_back();

        if (node.dir == Dir::Right)
            ++node.x;
        else if (node.dir == Dir::Up)
            --node.y;
        else if (node.dir == Dir::Left)
            --node.x;
        else
            ++node.y;

        if (node.x >= map.front().size() || node.y >= map.size())
            continue;

        node.acc_heat_loss += map[node.y][node.x];
        if (node.x == map.front().size() - 1 && node.y == map.size() - 1)
            return std::to_string(node.acc_heat_loss); // 771

        ++node.num_steps;
        node.min_remaining = manhattan(node.x, node.y, map.front().size(), map.size());
        if (node.num_steps < 3)
            add_to_queue(queue, node);

        node.dir = Dir((std::to_underlying(node.dir) + 1) % 4);
        node.num_steps = 0;
        add_to_queue(queue, node);

        node.dir = Dir((std::to_underlying(node.dir) + 2) % 4);
        add_to_queue(queue, node);
    }
}

std::string part2()
{
    Map map = get_input();
    auto max_dist = manhattan(0, 0, map.size(), map.front().size());
    std::vector<Entry> queue({
        { 0, 0, Dir::Right, 0, 0, max_dist },
        { 0, 0, Dir::Down, 0, 0, max_dist },
    });

    for (;;) {
        auto node = queue[0];
        std::pop_heap(queue.begin(), queue.end(), heap_compare);
        queue.pop_back();

        if (node.dir == Dir::Right)
            ++node.x;
        else if (node.dir == Dir::Up)
            --node.y;
        else if (node.dir == Dir::Left)
            --node.x;
        else
            ++node.y;

        ++node.num_steps;
        if (node.x >= map.front().size() || node.y >= map.size())
            continue;

        node.acc_heat_loss += map[node.y][node.x];
        if (node.x == map.front().size() - 1 && node.y == map.size() - 1 && node.num_steps >= 4)
            return std::to_string(node.acc_heat_loss); // 930

        node.min_remaining = manhattan(node.x, node.y, map.front().size(), map.size());
        if (node.num_steps < 10)
            add_to_queue(queue, node);

        if (node.num_steps >= 4) {
            node.dir = Dir((std::to_underlying(node.dir) + 1) % 4);
            node.num_steps = 0;
            add_to_queue(queue, node);

            node.dir = Dir((std::to_underlying(node.dir) + 2) % 4);
            add_to_queue(queue, node);
        }
    }
}

}
