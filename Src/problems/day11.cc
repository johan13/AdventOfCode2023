#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace day11 {

struct Pos { int64_t x; int64_t y; };

static auto get_input()
{
    std::vector<Pos> galaxies;
    std::ifstream file("input11.txt");
    std::string line;
    for (int64_t y = 0; std::getline(file, line); ++y) {
        for (int64_t x = line.find('#'); x != std::string::npos; x = line.find('#', x + 1))
            galaxies.push_back({ x, y });
    }

    return galaxies;
}

template<int64_t Pos::*axis>
static void expand(std::vector<Pos> &sorted_galaxies, int factor)
{
    int64_t previous_coord = 0, expand_amount = 0;
    for (Pos &galaxy : sorted_galaxies) {
        expand_amount += std::max(0ll, galaxy.*axis - previous_coord - 1) * (factor - 1);
        previous_coord = galaxy.*axis;
        galaxy.*axis += expand_amount;
    }
}

static constexpr int64_t manhattan(const Pos &a, const Pos &b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

static auto solution(int factor)
{
    auto galaxies = get_input();
    expand<&Pos::y>(galaxies, factor);
    std::ranges::sort(galaxies, [](const Pos &a, const Pos &b) { return a.x < b.x; });
    expand<&Pos::x>(galaxies, factor);

    int64_t answer = 0;
    for (size_t i = 1; i < galaxies.size(); ++i) {
        for (size_t j = 0; j < i; ++j)
            answer += manhattan(galaxies[i], galaxies[j]);
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
