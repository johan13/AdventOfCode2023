#include <algorithm>
#include <functional>
#include <string>
#include <stdio.h>
#include <tuple>
#include <vector>
#include "problems.h"
#include "../gfx.h"

namespace day01 { std::string part1(); std::string part2(); }
namespace day02 { std::string part1(); std::string part2(); }
namespace day03 { std::string part1(); std::string part2(); }
namespace day04 { std::string part1(); std::string part2(); }
namespace day05 { std::string part1(); std::string part2(); }
namespace day06 { std::string part1(); std::string part2(); }
namespace day07 { std::string part1(); std::string part2(); }
namespace day08 { std::string part1(); std::string part2(); }
namespace day09 { std::string part1(); std::string part2(); }

std::vector<std::tuple<std::function<std::string()>, std::function<std::string()>>> days{
    {day01::part1, day01::part2},
    {day02::part1, day02::part2},
    {day03::part1, day03::part2},
    {day04::part1, day04::part2},
    {day05::part1, day05::part2},
    {day06::part1, day06::part2},
    {day07::part1, day07::part2},
    {day08::part1, day08::part2},
    {day09::part1, day09::part2},
};

void do_problems()
{
    draw_header();

    for (int i = 0; i < (int)days.size(); i++)
    {
        printf("\n");
        set_x_pos(155);
        printf("DAY %02d\n", i + 1);

        auto [p1, p2] = days[i];

        std::string answer1 = p1();
        int width1 = width_of_string(answer1.c_str());
        set_x_pos(std::max(0, 100 - width1 / 2));
        printf("%s", answer1.c_str());
        fflush(stdout);

        std::string answer2 = p2();
        int width2 = width_of_string(answer2.c_str());
        set_x_pos(std::min(300 - width2 / 2, 400 - width2));
        printf("%s\n", answer2.c_str());
    }
}
