#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

namespace day13 {

typedef std::vector<std::string> Map;

static std::vector<Map> get_input()
{
    std::ifstream file("input13.txt");
    std::vector<Map> maps;
    Map map;
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() == 0) {
            maps.push_back(map);
            map.clear();
        } else {
            map.push_back(line);
        }
    }

    maps.push_back(map);
    return maps;
}

static int columns_left_of_mirror(const Map &map, int num_smudges)
{
    for (int columns_left = 1; columns_left < (int)map.front().length(); ++columns_left) {
        int cols_to_compare = std::min(columns_left, (int)map.front().length() - columns_left);
        int num_mismatches = 0;
        for (int i = 0; num_mismatches <= num_smudges && i < cols_to_compare; ++i) {
            int c1 = columns_left - i - 1;
            int c2 = columns_left + i;
            for (int r = 0; num_mismatches <= num_smudges && r < (int)map.size(); ++r) {
                if (map[r][c1] != map[r][c2])
                    ++num_mismatches;
            }
        }

        if (num_mismatches == num_smudges)
            return columns_left;
    }

    return 0;
}

static int rows_above_mirror(const Map &map, int num_smudges)
{
    for (int rows_above = 1; rows_above < (int)map.size(); ++rows_above) {
        int rows_to_compare = std::min(rows_above, (int)map.size() - rows_above);
        int num_mismatches = 0;
        for (int i = 0; num_mismatches <= num_smudges && i < rows_to_compare; ++i) {
            int r1 = rows_above - i - 1;
            int r2 = rows_above + i;
            for (int c = 0; num_mismatches <= num_smudges && c < (int)map.front().length(); ++c) {
                if (map[r1][c] != map[r2][c])
                    ++num_mismatches;
            }
        }

        if (num_mismatches == num_smudges)
            return rows_above;
    }

    return 0;
}

static std::string solution(int num_smudges)
{
    int answer = 0;
    for (const Map &map : get_input())
        answer += columns_left_of_mirror(map, num_smudges) + 100 * rows_above_mirror(map, num_smudges);

    return std::to_string(answer);
}

std::string part1()
{
    return solution(0); // 29213
}

std::string part2()
{
    return solution(1); // 37453
}

}
