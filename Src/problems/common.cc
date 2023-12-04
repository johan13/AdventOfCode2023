#include <cstring>
#include <regex>
#include "common.h"

std::vector<std::string> do_regex_match(const std::string &regex, const std::string &input)
{
    std::smatch match;
    std::regex_match(input, match, std::regex(regex));
    std::vector<std::string> groups;

    for (std::size_t i = 1; i < match.size(); i++) {
        groups.push_back(match[i].str());
    }

    return groups;
}

std::vector<std::string> split_string(const std::string &input, const char *separator)
{
    const std::size_t separator_length = std::strlen(separator);
    std::vector<std::string> parts;
    for (std::size_t start = 0;;) {
        std::size_t end = input.find(separator, start);
        parts.push_back(input.substr(start, end - start));
        if (end == std::string::npos)
            return parts;

        start = end + separator_length;
    }
}
