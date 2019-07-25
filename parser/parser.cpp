#include "parser.h"

#include <stdexcept>

LL1Parser::LL1Parser(Grammar grammar)
    : _grammar{ grammar }
{
    // construct parse table and check for first/follow conflicts
    // throw std::invalid_argument{ "grammar is not LL1" };
}

void LL1Parser::process(std::filesystem::path filepath)
{
    // stream = if file
    // vec<tokens> = _lexer.process(stream);
    //
}