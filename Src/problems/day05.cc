#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace day05 {

struct map_range_t {
    uint32_t destination;
    uint32_t source;
    uint32_t length;
};

struct almanac_t {
    std::vector<uint32_t> seeds;
    std::vector<std::vector<struct map_range_t>> maps;
};

static std::vector<struct map_range_t> read_map(std::ifstream &file)
{
    std::vector<struct map_range_t> map;
    for (;;) {
        uint32_t destination, source, length;
        file >> destination;
        file >> source;
        file >> length;
        if (file.fail()) {
            file.clear();
            return map;
        }

        map.push_back({destination, source, length});
    }
}

static struct almanac_t read_input()
{
    almanac_t almanac;
    std::ifstream file("input05.txt");
    std::string str;

    file >> str; // Skip "seeds:"
    for (;;) {
        uint32_t seed;
        file >> seed;
        if (file.fail()) {
            file.clear();
            break;
        }

        almanac.seeds.push_back(seed);
    }

    for (;;) {
        std::string ignored_header;
        std::getline(file, ignored_header);
        if (file.eof())
            break;

        almanac.maps.push_back(read_map(file));
    }

    return almanac;
}

std::string part1()
{
    auto almanac = read_input();
    uint32_t min_location = std::numeric_limits<uint32_t>::max();
    for (uint32_t seed : almanac.seeds) {
        for (auto map : almanac.maps) {
            for (auto range : map) {
                if (seed >= range.source && seed < range.source + range.length) {
                    seed += range.destination - range.source;
                    break;
                }
            }
        }

        min_location = std::min(min_location, seed);
    }

    return std::to_string(min_location); // 165788812
}

typedef std::vector<std::vector<struct map_range_t>>::const_iterator map_it;

static uint32_t find_min(uint32_t seed_start, uint32_t seed_len, map_it map, map_it map_end)
{
    if (map == map_end)
        return seed_start;

    uint32_t min_location = std::numeric_limits<uint32_t>::max();
    for (const auto &map_range : *map) {
        uint32_t intersection_start = std::max(map_range.source, seed_start);
        uint32_t intersection_end = std::min(map_range.source + map_range.length, seed_start + seed_len);
        if (intersection_start < intersection_end) {
            uint32_t new_min = find_min(
                intersection_start + map_range.destination - map_range.source,
                intersection_end - intersection_start,
                map + 1,
                map_end);
            min_location = std::min(min_location, new_min);
        }
    }

    return min_location;
}

std::string part2()
{
    auto almanac = read_input();

    // Sort the entries in each mapping by source and add no-op mappings to fill the gaps.
    for (auto &map : almanac.maps) {
        std::sort(map.begin(), map.end(), [](auto const &a, auto const &b) {
            return a.source < b.source;
        });
        
        uint32_t previous_end = 0;
        for (std::size_t i = 0; i < map.size(); i++) {
            if (map[i].source > previous_end) {
                map.insert(map.begin() + i, {previous_end, previous_end, map[i].source - previous_end});
                i++;
            }

            previous_end = map[i].source + map[i].length;
        }

        if (previous_end != 0)
            map.push_back({previous_end, previous_end, -previous_end});
    }

    uint32_t answer = std::numeric_limits<uint32_t>::max();
    for (std::size_t i = 0; i < almanac.seeds.size() - 1; i += 2) {
        uint32_t min = find_min(
            almanac.seeds[i],
            almanac.seeds[i + 1],
            almanac.maps.begin(),
            almanac.maps.end()
        );
        answer = std::min(answer, min);
    }

    return std::to_string(answer); // 1928058
}

}
