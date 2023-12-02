#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <ranges>
#include <format>
#include <optional>
#include <functional>
#include <array>

static std::array<std::string_view, 9> forward_lookup = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
static std::array<std::string_view, 9> reverse_lookup = {"eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin"};

template<typename Iter>
int check(int val, Iter first, Iter last, std::ranges::view auto numbers) {
    if (val != -1) {
        return val;
    }
    if (auto v = *first; v >= '0' && v <= '9') {
        return v - '0';
    }
    for (auto [i, num] : numbers | std::views::enumerate) {
        auto [s, n] = std::mismatch(first, last, num.begin(), num.end());
        if (std::distance(first, s) == num.length() && n == num.end()) {
            return i + 1;
        }
    }
    return -1;
}
int main() {    
    auto to_int = [&](std::string_view data) {
        auto first  = -1;
        auto second = -1;
        for (auto i = 0; i < data.size(); i++) {
            first  = check(first, data.begin() + i, data.end(), std::views::all(forward_lookup));
            second = check(second, data.rbegin() + i, data.rend(), std::views::all(reverse_lookup));
        }
        return first * 10 + second;
    };
    auto result = 0;
    for (auto i : std::views::istream<std::string>(std::cin) | std::views::transform(to_int)) {
        result += i;
    }
    std::cout << std::format("{}\n", result);
    return 0;
}


