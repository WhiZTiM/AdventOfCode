#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>
#include <sstream>

int main() {    
    auto pipeline = 
          std::views::filter([](auto c){ return c >= '0' && c <= '9'; })
        | std::views::transform([](auto c){ return c - '0'; })
        | std::views::take(1);

    auto to_int = [&](std::string data) {
        auto first = *(data | pipeline).begin();
        auto second = *(data | std::views::reverse | pipeline).begin();
        return first * 10 + second;
    };

    auto result = 0;
    for (auto i : std::views::istream<std::string>(std::cin) | std::views::transform(to_int)) {
        result += i;
    }
    std::cout << std::format("{}\n", result);
    return 0;
}
