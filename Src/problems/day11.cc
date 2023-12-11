#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace day11 {

struct Pos {
    int64_t x;
    int64_t y;
};

static auto solution(int factor)
{
    std::vector<Pos> galaxies;
    std::ifstream file("input11.txt");
    std::string line;
    for (int64_t y = 0; std::getline(file, line); ++y) {
        for (int64_t x = line.find('#'); x != std::string::npos; x = line.find('#', x + 1))
            galaxies.push_back({ x, y });
    }

    int64_t previous = 0;
    int64_t stretch = 0;
    for (Pos &g : galaxies) {
        stretch += std::max(0ll, g.y - previous - 1) * (factor - 1);
        previous = g.y;
        g.y += stretch;
    }

    std::sort(galaxies.begin(), galaxies.end(), [](const Pos &a, const Pos &b) { return a.x < b.x; });
    previous = 0;
    stretch = 0;
    for (Pos &g : galaxies) {
        stretch += std::max(0ll, g.x - previous - 1) * (factor - 1);
        previous = g.x;
        g.x += stretch;
    }

    int64_t answer = 0;
    for (size_t i = 1; i < galaxies.size(); ++i) {
        for (size_t j = 0; j < i; ++j)
            answer += std::abs(galaxies[i].x - galaxies[j].x) + std::abs(galaxies[i].y - galaxies[j].y);
    }

    return std::to_string(answer);
}

std::string part1()
{
    return solution(2); // 9565386
}

std::string part2()
{
    return solution(1000000); // 857986849428
}

}
