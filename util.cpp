#include "util.h"

std::vector<std::string> parseArguments(int argc, char* argv[])
{
    std::vector<std::string> arguments;

    for (int i = 0; i < argc; ++i)
        arguments.push_back(std::string{argv[i]});

    return arguments;
}