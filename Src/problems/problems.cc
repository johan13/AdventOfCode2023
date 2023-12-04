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

std::vector<std::tuple<std::function<std::string()>, std::function<std::string()>>> days{
    {day01::part1, day01::part2},
    {day02::part1, day02::part2},
    {day03::part1, day03::part2},
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
        set_x_pos(100 - width1 / 2);
        printf("%s", answer1.c_str());
        fflush(stdout);

        std::string answer2 = p2();
        int width2 = width_of_string(answer2.c_str());
        set_x_pos(300 - width2 / 2);
        printf("%s\n", answer2.c_str());
    }
}
