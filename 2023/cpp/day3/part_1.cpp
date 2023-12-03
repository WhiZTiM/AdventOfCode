#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>

static std::array<std::array<long, 2>, 8> direction = {{
    {1, 0},  {1, -1},  {1, 1},
    {-1, 0}, {-1, -1}, {-1, 1},
    {0, 1},  {0, -1},
}};

int main() {
    auto board = std::vector<std::vector<char>>{};
    std::ranges::for_each(std::views::istream<std::string>(std::cin), [&board](auto str) { 
        std::ranges::copy(str, std::back_inserter(board.emplace_back())); 
    });

    auto is_num =  [](auto c){ return c >= '0' && c <= '9'; };
    auto has_symbol  = [&, &b = board](std::array<long, 2> loc){
        auto pipeline = direction 
        | std::views::transform([&](auto d){ return std::array<long, 2>{loc[0] + d[0], loc[1] + d[1]}; })
        | std::views::filter([&](auto c){ return c[0] >= 0 && c[0] < b.size(); })
        | std::views::filter([&](auto c){ return c[1] >= 0 && c[1] < b.back().size(); })
        | std::views::transform([&](auto c) { return b[c[0]][c[1]]; });
        return std::ranges::any_of(pipeline, [&](auto c) { return !is_num(c) && c != '.'; });
    };
  

    auto numbers = std::vector<std::tuple<int, bool>>{};
    std::ranges::for_each(board | std::views::enumerate, [&](auto row) { 
        auto val = 0; auto sym = false;
        std::ranges::for_each(std::get<1>(row) | std::views::enumerate, [&](auto col) mutable {
            if (is_num(std::get<1>(col))) {
                sym |= has_symbol({(int)std::get<0>(row), (int)std::get<0>(col)});
                val = val * 10 + (std::get<1>(col) - '0');
            } else if (val != 0) {
                numbers.emplace_back(val, sym);
                val = 0;
                sym = false;
            }
        });
        // pick last item
        if (val != 0) {
            numbers.emplace_back(val, sym);
        }
    });

    std::cout << std::format("Count: {}\n", std::ranges::count_if(numbers, [](auto c) { return std::get<0>(c) > 0; }));
    auto iter = numbers 
        | std::views::filter([](auto c){ return std::get<1>(c); }) 
        | std::views::transform([](auto c){ return std::get<0>(c); });
    auto result = std::accumulate(iter.begin(), iter.end(), 0);
    std::cout << std::format("{}\n", result);
    return 0;
}

//536262