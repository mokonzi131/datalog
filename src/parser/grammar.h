#pragma once

#include <vector>
#include "token.h"

class Grammar
{
public:
    Grammar(Variable startSymbol);

    void addTerminal(TokenType tokenType);
    void addProduction(std::pair<Variable, std::vector<Variable>> production);

    std::vector<TokenType> terminals() const;

private:
    std::vector<Variable> _variables;
    std::vector<TokenType> _terminals;
    Variable _startSymbol;
    std::vector<std::pair<Variable, std::vector<Variable>>> _productions;
};

class DatalogGrammarFactory
{
public:
    static Grammar createDatalogGrammar();
};