#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <span>
#include <string_view>
#include <string>
#include <vector>

namespace day19 {

// I need to do some char[4] instead of std::string to reduce memory usage.

struct Rule {
    char param; // x, m, a or s
    char op; // < or >
    uint16_t threshold;
    char target[4];

    Rule(std::string_view str) {
        param = str[0];
        op = str[1];
        threshold = std::stoi(&str[2]);
        int i = str.find(":");
        memset(target, 0, sizeof target);
        str.substr(i + 1).copy(target, sizeof target - 1);
    }
};

struct Workflow {
    char name[4];
    std::vector<Rule> rules;
    char default_target[4];

    Workflow(std::string_view str) {
        size_t i = str.find("{");
        memset(name, 0, sizeof name);
        str.substr(0, i).copy(name, sizeof name - 1);

        for (;;) {
            size_t start = i + 1;
            i = str.find(",", start);
            if (i == std::string_view::npos) {
                i = str.find("}", start);
                memset(default_target, 0, sizeof default_target);
                str.substr(start, i - start).copy(default_target, sizeof default_target - 1);
                return;
            }

            rules.push_back(Rule(str.substr(start, i - start)));
        }
    }
};

struct Part {
    uint16_t x;
    uint16_t m;
    uint16_t a;
    uint16_t s;

    Part(std::string_view str) {
        size_t i = str.find("x=");
        x = std::stoi(&str[i + 2]);
        i = str.find("m=");
        m = std::stoi(&str[i + 2]);
        i = str.find("a=");
        a = std::stoi(&str[i + 2]);
        i = str.find("s=");
        s = std::stoi(&str[i + 2]);
    }
};

static auto get_input()
{
    std::ifstream file("input19.txt");
    std::string line;
    std::vector<Workflow> workflows;
    while (std::getline(file, line) && line.length() > 0)
        workflows.push_back(Workflow(line));

    std::vector<Part> parts;
    while (std::getline(file, line))
        parts.push_back(Part(line));

    return std::make_pair(std::move(workflows), std::move(parts));
}

static bool is_accepted(const Part &part, const std::vector<Workflow> &workflows) {
    std::string wf_name = "in";

    while (wf_name != "R" && wf_name != "A") {
        const auto &wf = *std::find_if(workflows.begin(), workflows.end(), [&](const Workflow &wf) {
            return 0 == std::strcmp(wf.name, wf_name.c_str());
        });

        wf_name = wf.default_target;
        for (const auto &rule : wf.rules) {
            int val = rule.param == 'x' ? part.x :
                      rule.param == 'm' ? part.m :
                      rule.param == 'a' ? part.a :
                                          part.s;
            if ((rule.op == '<' && val < rule.threshold) || (rule.op == '>' && val > rule.threshold)) {
                wf_name = rule.target;
                break;
            }
        }
    }

    return wf_name == "A";
}

std::string part1()
{
    auto [workflows, parts] = get_input();

    int answer = 0;
    for (const auto &part : parts) {
        if (is_accepted(part, workflows))
            answer += part.x + part.m + part.a + part.s;
    }

    return std::to_string(answer); // 432434
}

struct Limits {
    uint16_t x_min = 1;
    uint16_t x_max = 4000;
    uint16_t m_min = 1;
    uint16_t m_max = 4000;
    uint16_t a_min = 1;
    uint16_t a_max = 4000;
    uint16_t s_min = 1;
    uint16_t s_max = 4000;
};

static int64_t get_combos(
    const std::vector<Workflow> &workflows,
    std::span<const Rule> rules,
    const char *default_target,
    Limits limits)
{
    if (limits.x_min > limits.x_max || limits.m_min > limits.m_max || limits.a_min > limits.a_max || limits.s_min > limits.s_max)
        return 0;

    if (!rules.empty()) {
        Limits limits_a(limits), limits_b(limits); // a - match, b - no match
        const Rule &rule = rules[0];
        if (rule.op == '>') {
            if (rule.param == 'x') {
                limits_a.x_min = std::max(limits_a.x_min, static_cast<uint16_t>(rule.threshold + 1));
                limits_b.x_max = std::min(limits_b.x_max, rule.threshold);
            } else if (rule.param == 'm') {
                limits_a.m_min = std::max(limits_a.m_min, static_cast<uint16_t>(rule.threshold + 1));
                limits_b.m_max = std::min(limits_b.m_max, rule.threshold);
            } else if (rule.param == 'a') {
                limits_a.a_min = std::max(limits_a.a_min, static_cast<uint16_t>(rule.threshold + 1));
                limits_b.a_max = std::min(limits_b.a_max, rule.threshold);
            } else {
                limits_a.s_min = std::max(limits_a.s_min, static_cast<uint16_t>(rule.threshold + 1));
                limits_b.s_max = std::min(limits_b.s_max, rule.threshold);
            }
        } else {
            if (rule.param == 'x') {
                limits_a.x_max = std::min(limits_a.x_max, static_cast<uint16_t>(rule.threshold - 1));
                limits_b.x_min = std::max(limits_b.x_min, rule.threshold);
            } else if (rule.param == 'm') {
                limits_a.m_max = std::min(limits_a.m_max, static_cast<uint16_t>(rule.threshold - 1));
                limits_b.m_min = std::max(limits_b.m_min, rule.threshold);
            } else if (rule.param == 'a') {
                limits_a.a_max = std::min(limits_a.a_max, static_cast<uint16_t>(rule.threshold - 1));
                limits_b.a_min = std::max(limits_b.a_min, rule.threshold);
            } else {
                limits_a.s_max = std::min(limits_a.s_max, static_cast<uint16_t>(rule.threshold - 1));
                limits_b.s_min = std::max(limits_b.s_min, rule.threshold);
            }
        }

        return
            get_combos(workflows, std::span<Rule>(), rule.target, limits_a) +
            get_combos(workflows, rules.subspan(1), default_target, limits_b);
    }


    if (0 == strcmp(default_target, "A")) {
        return (static_cast<int64_t>(limits.x_max) - limits.x_min + 1) *
            (static_cast<int64_t>(limits.m_max) - limits.m_min + 1) *
            (static_cast<int64_t>(limits.a_max) - limits.a_min + 1) *
            (static_cast<int64_t>(limits.s_max) - limits.s_min + 1);
    } else if (0 == strcmp(default_target, "R")) {
        return 0;
    } else {
        const Workflow &wf = *std::find_if(workflows.begin(), workflows.end(), [&](const Workflow &wf) {
            return 0 == std::strcmp(wf.name, default_target);
        });

        return get_combos(workflows, wf.rules, wf.default_target, limits);
    }
}

std::string part2()
{
    auto [workflows, parts] = get_input();

    int64_t answer = get_combos(workflows, std::span<Rule>(), "in", Limits());

    return std::to_string(answer); // 132557544578569
}

}
