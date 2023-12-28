#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace day20 {

struct Module {
    char type; // % or & (or \0 for broadcaster)
    std::string name;
    std::vector<std::string> destinations;
    bool state; // For flip-flops
    std::map<std::string, bool> inputs; // For conjunctions: true is most recent pulse was positive
};

static auto get_input()
{
    std::vector<Module> modules;
    std::ifstream file("input20.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto &module = modules.emplace_back();
        if (line.starts_with("broadcaster -> ")) {
            module.name = "broadcaster";
        } else {
            module.type = line[0];
            int i = line.find(' ');
            module.name = line.substr(1, i - 1);
        }

        size_t i = line.find(" -> ") + 4;
        for (;;) {
            size_t j = line.find(", ", i);
            if (j == std::string::npos) {
                module.destinations.push_back(line.substr(i));
                break;
            } else {
                module.destinations.push_back(line.substr(i, j - i));
                i = j + 2;
            }
        }
    }

    return modules;
}

std::string part1()
{
    auto input = get_input();
    std::cerr << input.size() << ' ' << input.front().type << ' ' << input.front().name << ' '
        << input.front().destinations.size() << ' ' << input.front().destinations.back() << '\n';
    return "TODO";
}

std::string part2()
{
    return "TODO";
}

}
