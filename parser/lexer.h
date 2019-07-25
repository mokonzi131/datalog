#pragma once

#include <vector>
#include <fstream>
#include "token.h"

class Lexer
{
public:
    Lexer(std::vector<TokenType> types);
    std::vector<Token> process(std::istream& data);

private:
    std::vector<TokenType> _types;
};