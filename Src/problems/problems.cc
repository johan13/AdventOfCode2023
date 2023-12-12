#include <algorithm>
#include <cstdio>
#include <functional>
#include <string>
#include <tuple>
#include <vector>
#include "problems.h"
#include "../gfx.h"

extern int vsync_count;

namespace day01 { std::string part1(); std::string part2(); }
namespace day02 { std::string part1(); std::string part2(); }
namespace day03 { std::string part1(); std::string part2(); }
namespace day04 { std::string part1(); std::string part2(); }
namespace day05 { std::string part1(); std::string part2(); }
namespace day06 { std::string part1(); std::string part2(); }
namespace day07 { std::string part1(); std::string part2(); }
namespace day08 { std::string part1(); std::string part2(); }
namespace day09 { std::string part1(); std::string part2(); }
namespace day10 { std::string part1(); std::string part2(); }
namespace day11 { std::string part1(); std::string part2(); }
namespace day12 { std::string part1(); std::string part2(); }

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
    {day10::part1, day10::part2},
    {day11::part1, day11::part2},
    {day12::part1, day12::part2},
};

void do_problems()
{
    draw_header();

    fprintf(stderr, "Starting problems...\n");
    int start_count = vsync_count;
    int prev_count = start_count;
    for (int i = 0; i < (int)days.size(); i++) {
        printf("\n");
        set_x_pos(155);
        printf("DAY %02d\n", i + 1);

        auto [part1, part2] = days[i];

        std::string answer1 = part1();
        int width1 = width_of_string(answer1.c_str());
        set_x_pos(std::max(100 - width1 / 2, 0));
        printf("%s", answer1.c_str());
        fflush(stdout); // No newline, so we must flush before set_x_pos.

        std::string answer2 = part2();
        int width2 = width_of_string(answer2.c_str());
        set_x_pos(std::min(300 - width2 / 2, 400 - width2));
        printf("%s\n", answer2.c_str());

        int end_count = vsync_count;
        fprintf(stderr, "Solved day %02d in %d ms.\n", i + 1, (end_count - prev_count) * 25);
        prev_count = end_count;
    }

    fprintf(stderr, "Solved all problems in %d ms.\n", (prev_count - start_count) * 25);
}
