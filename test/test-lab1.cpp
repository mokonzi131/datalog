#include <fstream>
#include "catch2/catch.hpp"
#include "src/parser/grammar.h"
#include "src/parser/lexer.h"

SCENARIO("lab 1 examples are working", "[lab1]") {
    Grammar datalogGrammar = DatalogGrammarFactory::createDatalogGrammar();
    Lexer lex{ datalogGrammar.terminals() };

    GIVEN("the example 1 file") {
        std::ifstream data{ "./examples/example1.txt", std::ios::in };
        auto tokens = lex.process(data);

        THEN("the number of tokens generated should be 16") { REQUIRE(tokens.size() == 16); }

        WHEN("the tokens are filtered to remove whitespace") {
            std::vector<Token> filtered;
            std::copy_if(std::cbegin(tokens), std::cend(tokens), std::back_inserter(filtered), [](const Token& token) {
                return token.type().name() != TokenType::Whitespace.name();
            });

            THEN("the number of tokens remaining should be 9") { REQUIRE(filtered.size() == 9); }
        }
    }

    GIVEN("the example 2 file") {
        std::ifstream data{ "./examples/example2.txt", std::ios::in };
        auto tokens = lex.process(data);

        THEN("the number of tokens generated should be 21") { REQUIRE(tokens.size() == 21); }

        WHEN("the tokens are filtered to remove whitespace") {
            std::vector<Token> filtered;
            std::copy_if(std::cbegin(tokens), std::cend(tokens), std::back_inserter(filtered), [](const Token& token) {
                return token.type().name() != TokenType::Whitespace.name();
            });

            THEN("the number of tokens remaining should be 16") { REQUIRE(filtered.size() == 16); }
        }
    }

    GIVEN("the example 3 file") {
        std::ifstream data{ "./examples/example3.txt", std::ios::in };
        auto tokens = lex.process(data);

        THEN("the number of tokens generated should be 16") { REQUIRE(tokens.size() == 34); }

        WHEN("the tokens are filtered to remove whitespace") {
            std::vector<Token> filtered;
            std::copy_if(std::cbegin(tokens), std::cend(tokens), std::back_inserter(filtered), [](const Token& token) {
                return token.type().name() != TokenType::Whitespace.name();
            });

            THEN("the number of tokens remaining should be 9") { REQUIRE(filtered.size() == 24); }
        }
    }
}