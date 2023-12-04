#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>
#include <unordered_set>

int main() {
    auto result = 0LL;
    for (auto line = std::string{}; std::getline(std::cin, line);) {
        auto ours = std::vector<int>{};
        auto win  = std::unordered_set<int>{};

        auto to_int = [](auto r) { return std::stoi(std::string(r.begin(), r.end())); };
        auto remove_chunker = std::views::filter([](auto c){ return std::string_view(c) != "|"; });

        auto card = line | std::views::split(' ') | std::views::filter([](auto c){ return std::string_view(c) != ""; });
        auto right_data = card | std::views::drop(2) | std::views::chunk_by([](auto _, auto c){ return std::string_view(c) != "|"; });

        std::ranges::copy(
            (right_data | std::views::take(1)).front() | std::views::transform(to_int),
            std::back_inserter(ours)
        );
        std::ranges::copy(
            (right_data | std::views::drop(1) | std::views::take(1)).front() | remove_chunker | std::views::transform(to_int), 
            std::inserter(win, win.begin())
        );

        auto yes = ours | std::views::filter([&](auto v){ return win.contains(v); }) | std::views::transform([](auto c) { return 1; });
        if (std::ranges::distance(yes) > 0) {
            result += std::reduce(std::next(yes.begin()), yes.end(), 1LL, [](auto a, auto _){ return a * 2; });
        }
    }
    std::cout << std::format("\n{}\n", result);
    return 0;
}