#include <algorithm>
#include <fstream>
#include <numeric>
#include <vector>

namespace day09 {

static std::vector<std::vector<int>> get_input()
{
    std::ifstream file("input09.txt");
    std::string line;
    std::vector<std::vector<int>> sequences;
    while (std::getline(file, line)) {
        auto &sequence = sequences.emplace_back();
        for (std::size_t i = 0; i != std::string::npos; i = line.find(' ', i + 1))
            sequence.push_back(std::stoi(&line[i]));
    }

    return sequences;
}

template<typename Iter>
static int predict_next(Iter begin, Iter end)
{
    if (std::all_of(begin, end, [](int x) { return x == 0; }))
        return 0;

    std::vector<int> deltas;
    std::adjacent_difference(begin, end, std::back_inserter(deltas));
    int next_delta = predict_next(deltas.begin() + 1, deltas.end());

    return *(end - 1) + next_delta;
}

std::string part1()
{
    auto sequences = get_input();
    int answer = std::accumulate(sequences.begin(), sequences.end(), 0, [](int sum, const auto &seq) {
        return sum + predict_next(seq.begin(), seq.end());
    });

    return std::to_string(answer); // 1868368343
}

std::string part2()
{
    auto sequences = get_input();
    int answer = std::accumulate(sequences.begin(), sequences.end(), 0, [](int sum, const auto &seq) {
        return sum + predict_next(seq.rbegin(), seq.rend());
    });

    return std::to_string(answer); // 1022
}

}
