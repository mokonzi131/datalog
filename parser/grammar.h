#pragma once

#include <vector>
#include "token.h"

class Grammar
{
public:
    void addTerminal(TokenType tokenType);

    std::vector<TokenType> terminals() const;

private:
    // _variables???
    std::vector<TokenType> _terminals;
    // _start
    // _productions
};

class DatalogGrammarFactory
{
public:
    static Grammar createDatalogGrammar();
};