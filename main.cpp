#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <filesystem>
#include <algorithm>
#include <array>
#include "util.h"
#include "grammar.h"
#include "parser.h"
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

//    auto filename = args.at(1);

    //auto grammar = DatalogGrammarFactory::createDatalogGrammar();
    //LL1Parser parser{ grammar }; // checks grammar validity and if its LL1
    //auto tokens = parser.process(filename);
    
//    DatalogProgram db; // -> parser.loadProgram(filename)
//    db.load(parser, filename); // parser (lexes) and builds database structure
//    db.evaluate(); // evaluates the queries against the database

//    parser.loadProgram(filename);

//    Datalog database{ parser };
//    database.load(filename);

//    grammer.addToken("COMMA", [](std::ifstream& data){
//        if (data.peek() == ',') return std::tuple(true, std::string{","});
//        else return std::tuple(false, std::string{});
//    });
}
