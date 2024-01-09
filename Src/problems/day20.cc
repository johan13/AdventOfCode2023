#include <algorithm>
#include <cstdint>
#include <deque>
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

struct Pulse {
    std::string recipient;
    std::string sender;
    bool positive;
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

    // Initialize the inputs map of the conjuction modules.
    for (const Module &mod : input) {
        for (const std::string &dest_name : mod.destinations) {
            Module &dest = *std::find_if(input.begin(), input.end(), [&](const Module &m) {
                return m.name == dest_name;
            });
            if (dest.type == '&') dest.inputs[mod.name] = false;
        }
    }

    int num_negative = 0, num_positive = 0;
    for (int i = 0; i < 1000; ++i) {
        std::deque<Pulse> queue;
        queue.push_back({"broadcaster", "button", false});
        while (!queue.empty()) {
            const auto [name, sender, positive] = queue.front();
            queue.pop_front();
            if (positive)
                ++num_positive;
            else
                ++num_negative;

            Module &rec = *std::find_if(input.begin(), input.end(), [&](const Module &m) {
                return m.name == name;
            });

            if (rec.type == '%' && !positive) {
                rec.state = !rec.state;
                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, rec.state });
            } else if (rec.type == '&') {
                rec.inputs[sender] = positive;
                bool all_high = std::all_of(rec.inputs.begin(), rec.inputs.end(), [](const auto &x) {
                    return x.second;
                });

                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, !all_high });
            } else if (name == "broadcaster") {
                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, false });
            }
        }
    }

    return std::to_string(num_positive * num_negative); // 834323022
}

std::string part2()
{
    auto input = get_input();

    // Initialize the inputs map of the conjuction modules.
    for (const Module &mod : input) {
        for (const std::string &dest_name : mod.destinations) {
            Module &dest = *std::find_if(input.begin(), input.end(), [&](const Module &m) {
                return m.name == dest_name;
            });
            if (dest.type == '&') dest.inputs[mod.name] = false;
        }
    }

    /*
     * The rx node gets its input from rs which in turn gets its input from bt, dl, fr and rv.
     * bt, dl, fr and rv each send a negative pulse at regular intervals (n, 2n, 3n, ...) with
     * different (prime) periods. The product of these four periods is the answer.
     */

    int bt = 0, dl = 0, fr = 0, rv = 0;
    for (int i = 1;; ++i) {
        std::deque<Pulse> queue;
        queue.push_back({"broadcaster", "button", false});
        while (!queue.empty()) {
            const auto [name, sender, positive] = queue.front();
            queue.pop_front();

            if (!positive) {
                if (name == "bt") bt = i;
                if (name == "dl") dl = i;
                if (name == "fr") fr = i;
                if (name == "rv") rv = i;
                if (bt && dl && fr && rv)
                    return std::to_string((int64_t)bt * dl * fr * rv); // 225386464601017
            }

            Module &rec = *std::find_if(input.begin(), input.end(), [&](const Module &m) {
                return m.name == name;
            });

            if (rec.type == '%' && !positive) {
                rec.state = !rec.state;
                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, rec.state });
            } else if (rec.type == '&') {
                rec.inputs[sender] = positive;
                bool all_high = std::all_of(rec.inputs.begin(), rec.inputs.end(), [](const auto &x) {
                    return x.second;
                });

                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, !all_high });
            } else if (name == "broadcaster") {
                for (const std::string &dest : rec.destinations)
                    queue.push_back({ dest, name, false });
            }
        }
    }
}

}
