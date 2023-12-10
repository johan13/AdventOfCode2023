#include <fstream>
#include <string>
#include <vector>

namespace day10 {

enum struct Heading { Right, Up, Left, Down };

struct Input {
    std::vector<std::string> map;
    int start_x;
    int start_y;
    Heading start_heading;
};

static Input get_input()
{
    std::ifstream file("input10.txt");
    std::vector<std::string> map;
    std::string line;
    while (std::getline(file, line))
        map.push_back(line);

    // Take a shortcut: The start is at 53,75 and should be a "-".
    map[75][53] = '-';
    return {map, 53, 75, Heading::Right};
}

std::string part1()
{
    auto [map, start_x, start_y, heading] = get_input();
    int x = start_x, y = start_y;
    int steps = 0;
    do {
        char c = map[y][x];
        if (c == 'L')
            heading = heading == Heading::Down ? Heading::Right : Heading::Up;
        else if (c == 'J')
            heading = heading == Heading::Down ? Heading::Left : Heading::Up;
        else if (c == '7')
            heading = heading == Heading::Right ? Heading::Down : Heading::Left;
        else if (c == 'F')
            heading = heading == Heading::Left ? Heading::Down : Heading::Right;

        ++steps;
        if (heading == Heading::Right)
            ++x;
        else if (heading == Heading::Up)
            --y;
        else if (heading == Heading::Left)
            --x;
        else
            ++y;
    } while (x != start_x || y != start_y);

    return std::to_string(steps / 2); // 6682
}

static constexpr char visited(char c) { return c | 0x80; } // Set the high bit on visited squares.

std::string part2()
{
    auto [map, start_x, start_y, heading] = get_input();
    int x = start_x, y = start_y;
    do {
        char c = map[y][x];
        map[y][x] = visited(c);
        if (c == 'L')
            heading = heading == Heading::Down ? Heading::Right : Heading::Up;
        else if (c == 'J')
            heading = heading == Heading::Down ? Heading::Left : Heading::Up;
        else if (c == '7')
            heading = heading == Heading::Right ? Heading::Down : Heading::Left;
        else if (c == 'F')
            heading = heading == Heading::Left ? Heading::Down : Heading::Right;

        if (heading == Heading::Right)
            ++x;
        else if (heading == Heading::Up)
            --y;
        else if (heading == Heading::Left)
            --x;
        else
            ++y;
    } while (x != start_x || y != start_y);

    int num_inside = 0;
    enum { Outside, Inside, BorderInsideUp, BorderInsideDown } state = Outside;
    for (const auto &row : map) {
        for (char c : row) {
            if (c == visited('|'))
                state = state == Outside ? Inside : Outside;
            else if (c == visited('L'))
                state = state == Outside ? BorderInsideUp : BorderInsideDown;
            else if (c == visited('F'))
                state = state == Outside ? BorderInsideDown : BorderInsideUp;
            else if (c == visited('J'))
                state = state == BorderInsideUp ? Outside : Inside;
            else if (c == visited('7'))
                state = state == BorderInsideUp ? Inside : Outside;
            else if (state == Inside)
                ++num_inside;
        }
    }

    return std::to_string(num_inside); // 353
}

}
