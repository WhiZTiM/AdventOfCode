#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <ranges>
#include <format>
#include <numeric>
#include <sstream>


// This solution could be much shorter, just wanted to experiment with tokenization based approach

struct Game {
    int id = 0;
    std::vector<std::array<int, 3>> cubes = {}; // Red, Blue, Green
};

struct Lexer {
    std::vector<std::string_view> tokens;

    void lex(std::string_view data) {
        tokens.clear();
        auto shouldConsume = [](char c) { 
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
        };
        
        for (auto base = 0, iter = 0; base < data.size(); base = iter = iter + 1) {
            if (data[base] == ' ') {
                continue;
            }
            for (;iter != data.size() && shouldConsume(data[iter]); iter++);
            tokens.push_back(data.substr(base, iter - base));
            if (data[iter] == ';') {
                tokens.push_back(data.substr(iter, 1));
            }
        }
    }
};

Game parse_line(const std::vector<std::string_view>& tokens) {
    auto value = 0;
    auto game = Game{};
    game.cubes.push_back({});

    // 1 parse game.id
    std::from_chars(tokens[1].data(), tokens[1].data() + tokens[1].size(), game.id);

    for (auto i = 2; i < tokens.size(); i++) {
        // 2 Eagerly push new draw; Token consumption done by loop control
        if (tokens[i] == ";") {
            game.cubes.push_back({});
            continue;
        }

        // 3. Parse number; Token consumption done manually
        std::from_chars(tokens[i].data(), tokens[i].data() + tokens[i].size(), value);
        i += 1;

        // 3. Parse type; Token consumption done by loop control
        if (tokens[i] == "red") {
            game.cubes.back()[0] = value;
        } else if (tokens[i] == "green") {
            game.cubes.back()[1] = value;
        } else if (tokens[i] == "blue") {
            game.cubes.back()[2] = value;
        }
    }

    return game;
}

int main() {    
    auto result = 0ULL;
    auto lexer = Lexer{};
    auto line = std::string{};
    while (std::getline(std::cin, line)) {
        lexer.lex(line);
        auto game = parse_line(lexer.tokens);
        auto cubes = std::array<int, 3>{};
        for (auto c : game.cubes) {
            cubes[0] = std::max(cubes[0], c[0]);
            cubes[1] = std::max(cubes[1], c[1]);
            cubes[2] = std::max(cubes[2], c[2]);
        }
        result += 1ULL * cubes[0] * cubes[1] * cubes[2];
        if (result > 9'000'000'000) {
            std::cout << "Has overflow!\n"; /// not sure how to implement BigInteger yet
        }
    }
    std::cout << std::format("\nResult: {}\n", result);
    return 0;
}
