#pragma once

#include <filesystem>
#include <string>
#include "grammar.h"
#include "lexer.h"

class LL1Parser
{
public:
    LL1Parser(Grammar grammar);
    void process(std::filesystem::path filepath);

private:
    Grammar _grammar;
    
};