#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>
#include <unordered_set>

static std::array<std::array<long, 2>, 8> direction = {{
    {1, 0},  {1, -1},  {1, 1},
    {-1, 0}, {-1, -1}, {-1, 1},
    {0, 1},  {0, -1},
}};

int main() {
    auto board = std::vector<std::vector<char>>{};
    auto gears = std::vector<std::vector<std::vector<long>>>{};
    std::ranges::for_each(std::views::istream<std::string>(std::cin), [&](auto str) { 
        std::ranges::copy(str, std::back_inserter(board.emplace_back())); 
        std::ranges::transform(str, std::back_inserter(gears.emplace_back()), [](auto c){ return std::vector<long>{}; });
    });

    auto broadcast_number = [&](auto& cache, auto num, auto y, auto x, auto x_end) {        
        auto pipeline = std::views::transform([&](auto d){ return std::array<long, 2>{y + d[0], x + d[1]}; })
        | std::views::filter([&](auto c){ return c[0] >= 0 && c[0] < (int) board.size(); })
        | std::views::filter([&](auto c){ return c[1] >= 0 && c[1] < (int) board.back().size(); })
        | std::views::filter([&](auto c){ 
            if (board[c[0]][c[1]] != '*') {
                return false;
            }
            auto [_, inserted] = cache.insert(c[0] * board.size() + c[1]);
            return inserted;
        });

        std::ranges::for_each(std::views::iota(x, x_end), [&](auto _) {
            std::ranges::for_each(direction | pipeline, [&](auto c) {
                gears[c[0]][c[1]].push_back(num); 
            });
            x += 1; // Not the best idea to mutate this far away from usage points ¯\_(ツ)_/¯
        });
    };

    auto is_num =  [](auto c){ return c >= '0' && c <= '9'; };
    std::ranges::for_each(board | std::views::enumerate, [&](auto row_pair) {
        auto& [y, chars] = row_pair;
        auto val = 0, stop = 0;
        auto cache = std::unordered_set<bool>{};
        std::ranges::for_each(chars | std::views::enumerate, [&](auto character_pair) mutable {
            auto& [x, char_] = character_pair;
            if (!is_num(char_) || x < stop) {
                return;
            }
            stop = std::distance(chars.begin(), std::ranges::find_if_not(chars.begin() + x, chars.end(), is_num));
            std::from_chars(chars.data() + x, chars.data() + stop, val);
            broadcast_number(cache, val, y, x, stop);
            cache.clear();
        });
    });

    auto ratios = gears | std::views::join
        | std::views::filter([](auto g) { return g.size() == 2; })
        | std::views::transform([](auto g){ return g[0] * g[1]; });
    std::cout << std::format("{}\n", std::accumulate(ratios.begin(), ratios.end(), 0L));
    return 0;
}