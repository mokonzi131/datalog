#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <filesystem>
#include <algorithm>
#include <array>
#include "util.h"
#include "parser/grammar.h"
#include "parser/parser.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    auto args = parseArguments(argc, argv);
    if (args.size() != 2) {
        std::cout << "USAGE: lexer <filename>\n";
        return EXIT_FAILURE;
    }

    fs::path filepath{ args.at(1) };
    if (!fs::exists(filepath))
    {
        std::cout << "FILE: " << filepath.string() << " not found.\n";
        return EXIT_FAILURE;
    }

    Grammar datalogGrammar = DatalogGrammarFactory::createDatalogGrammar();

    std::ifstream data{ filepath, std::ios::in };
    Lexer lex{ datalogGrammar.terminals() };
    auto tokens = lex.process(data);
    
    std::vector<Token> filtered;
    std::copy_if(std::cbegin(tokens), std::cend(tokens), std::back_inserter(filtered), [](const Token& token) {
        return token.type().name() != TokenType::Whitespace.name();
    });
    for (const auto& token : filtered)
    {
        std::cout << token << "\n";
    }
    std::cout << "Total Tokens = " << filtered.size() << "\n";
}
