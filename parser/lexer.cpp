#include "lexer.h"
#include <algorithm>
#include <iostream>

Lexer::Lexer(std::vector<TokenType> types)
    : _types{ types }
{
    _types.insert(std::begin(_types), TokenType::EndOfFile);
    _types.push_back(TokenType::Whitespace);
    _types.push_back(TokenType::Undefined);
}

std::vector<Token> Lexer::process(std::istream& data)
{
    std::vector<Token> tokens;

    size_t line = 1;
    while (!data.eof())
    {
        auto it = std::find_if(std::cbegin(_types), std::cend(_types), [&data](const TokenType& type) {
            return type.matchesNext(data);
        });
        if (it != std::cend(_types))
        {
            auto value = it->extractNext(data);

            bool unexpectedEof = data.eof() && it->name() != TokenType::EndOfFile.name();
            if (unexpectedEof)
                tokens.push_back(Token{ TokenType::Undefined, value, line });
            else
                tokens.push_back(Token{ *it, value, line });   

            line += std::count(std::cbegin(value), std::cend(value), '\n');

            if (unexpectedEof)
                tokens.push_back(Token{ TokenType::EndOfFile, std::string{}, line });
        }
        else
        {
            throw std::runtime_error{ "corrupt stream data" };
        }
    }

    return tokens;
}