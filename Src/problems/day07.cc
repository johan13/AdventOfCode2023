#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace day07 {

enum struct Type { HighCard, OnePair, TwoPairs, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind };

struct Hand {
    std::string cards;
    int bid;
    Type type;
};

static Type get_type(const std::string &cards, bool j_is_joker)
{
    int jokers = 0;
    std::map<char, int> map;
    for (char card : cards) {
        if (j_is_joker && card == 'J')
            jokers++;
        else
            map[card]++;
    }
    std::vector<int> counts;
    std::transform(map.begin(), map.end(), std::back_inserter(counts),
        [](auto const &x) { return x.second; });
    std::sort(counts.rbegin(), counts.rend());

    if (jokers == 5 || counts[0] + jokers == 5)
        return Type::FiveOfAKind;
    if (counts[0] + jokers == 4)
        return Type::FourOfAKind;
    if (counts[0] + jokers == 3)
        return counts[1] == 2 ? Type::FullHouse : Type::ThreeOfAKind;
    if (counts[0] + jokers == 2)
        return counts[1] == 2 ? Type::TwoPairs : Type::OnePair;
    return Type::HighCard;
}

static std::string solution(bool j_is_joker)
{
    std::ifstream file("input07.txt");
    std::string line;
    std::vector<Hand> hands;
    while (std::getline(file, line)) {
        Hand &hand = hands.emplace_back();
        hand.cards = line.substr(0, 5);
        hand.bid = std::stoi(&line[6]);
        hand.type = get_type(hand.cards, j_is_joker);
    }

    std::map<char, int> card_values{
        {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'T', 10}, {'J', j_is_joker ? 0 : 11}, {'Q', 12}, {'K', 13}, {'A', 14}};
    std::sort(hands.begin(), hands.end(), [&card_values](const Hand &a, const Hand &b) {
        if (a.type != b.type)
            return a.type < b.type;
        for (int i = 0;; i++) {
            if (a.cards[i] != b.cards[i])
                return card_values[a.cards[i]] < card_values[b.cards[i]];
        }
    });

    int answer = 0;
    for (std::size_t i = 0; i < hands.size(); i++)
        answer += (i + 1) * hands[i].bid;

    return std::to_string(answer);
}

std::string part1()
{
    return solution(false); // 248569531
}

std::string part2()
{
    return solution(true); // 250382098
}

}
