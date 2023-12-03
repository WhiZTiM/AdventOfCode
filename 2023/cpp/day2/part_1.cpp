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

std::ostream& operator << (std::ostream& os, const Game& g) {
    os << std::format("GameId:{}\t", g.id);
    std::ranges::for_each(g.cubes, [&](auto a){ os << std::format("red:{}, green:{}, blue:{};  ", a[0], a[1], a[2]); });
    os << "\n";
    return os;
}

bool is_possible(const Game& g) {
    return std::ranges::all_of(
        g.cubes | std::views::transform([](auto a){ return a; }),
        [](auto a) { return a[0] < 13 && a[1] < 14 && a[2] < 15; }
    );
}

int main() {    
    auto result = 0;
    auto lexer = Lexer{};
    auto line = std::string{};
    while (std::getline(std::cin, line)) {
        lexer.lex(line);
        auto game = parse_line(lexer.tokens);
        if (is_possible(game)) {
            result += game.id;
        }
    }
    std::cout << std::format("\nResult: {}\n", result);
    return 0;
}
