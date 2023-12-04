#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>
#include <unordered_set>

int main() {
    auto cards  = std::vector<int>{};
    auto lines  = std::vector<std::string>{};
    for (auto line  = std::string{}; std::getline(std::cin, line); lines.push_back(line), cards.push_back(1));
    
    for (auto [i, line] : lines | std::views::enumerate) {
        auto ours = std::vector<int>{};
        auto win  = std::unordered_set<int>{};

        auto to_int = [](auto r) { return std::stoi(std::string(r.begin(), r.end())); };
        auto remove_chunker = std::views::filter([](auto c){ return std::string_view(c) != "|"; });

        auto card = line | std::views::split(' ') | std::views::filter([](auto c){ return std::string_view(c) != ""; });
        auto records = card | std::views::drop(2) | std::views::chunk_by([](auto _, auto c){ return std::string_view(c) != "|"; });

        std::ranges::copy(
            (records | std::views::take(1)).front() | std::views::transform(to_int), 
            std::back_inserter(ours)
        );
        std::ranges::copy(
            (records | std::views::drop(1) | std::views::take(1)).front() | remove_chunker | std::views::transform(to_int), 
            std::inserter(win, win.begin())
        );

        auto yes = ours | std::views::filter([&](auto v){ return win.contains(v); });
        auto size = std::ranges::distance(yes);
        auto to_fill = std::views::iota(i + 1, i + 1 + size) | std::views::filter([&](auto c){ return c < (int) cards.size(); });
        std::ranges::for_each(to_fill, [&](auto c){ cards[c] += cards[i]; });
    }
    std::cout << std::format("\n{}\n", std::accumulate(cards.begin(), cards.end(), 0));
    return 0;
}