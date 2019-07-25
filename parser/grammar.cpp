#include "grammar.h"
#include <functional>
#include <string>

void Grammar::addTerminal(TokenType tokenType) { _terminals.push_back(tokenType); }

std::vector<TokenType> Grammar::terminals() const { return _terminals; }

bool checkSpecificCharacter(std::istream& stream, char character)
{
    return static_cast<char>(stream.peek()) == character;
}

bool checkIdentifier(std::istream& stream)
{
    return std::isalpha(stream.peek());
}

bool checkKeyword(std::istream& stream, std::string keyword)
{
    if (!checkIdentifier(stream)) return false;

    bool found = false;
    auto position = stream.tellg();

    auto it = std::begin(keyword);
    auto next = stream.get();
    while (it != std::end(keyword) &&
        next != std::char_traits<char>::eof() && 
        *it == static_cast<char>(next))
    {
        ++it;
        next = stream.get();
    }
    stream.unget();

    if (it == std::end(keyword) && !checkIdentifier(stream))
        found = true;

    stream.seekg(position);
    return found;
}

bool checkSequence(std::istream& stream, std::string sequence)
{
    bool found = false;
    auto position = stream.tellg();

    auto it = std::begin(sequence);
    auto next = stream.get();
    while (it != std::end(sequence) &&
        next != std::char_traits<char>::eof() &&
        *it == static_cast<char>(next))
    {
        ++it;
        next = stream.get();
    }

    if (it == std::end(sequence))
        found = true;

    stream.seekg(position);
    return found;
}

std::string getSingleCharacterAsString(std::istream& stream)
{
    return std::string{ static_cast<char>(stream.get()) };
}

std::string getIdentifier(std::istream& stream)
{
    std::string result;

    int next = stream.get();
    while (next != std::char_traits<char>::eof() && std::isalnum(next))
    {
        result.push_back(static_cast<char>(next));
        next = stream.get();
    }

    stream.unget();

    return result;
}

std::string getSequence(std::istream& stream, std::string sequence)
{
    stream.ignore(sequence.size());
    return sequence;
}

std::string getLineComment(std::istream& stream)
{
    std::string result;
    std::getline(stream, result);
    stream.unget();
    return result;
}

void appendThrough(std::istream& stream, std::string& value, char delim = '\n')
{
    std::string append;
    std::getline(stream, append, delim);
    append.push_back(delim);
    value += append;
}

std::string getBlockComment(std::istream& stream)
{
    std::string result;
    result.push_back(static_cast<char>(stream.get()));
    result.push_back(static_cast<char>(stream.get()));

    appendThrough(stream, result, '|');
    while (static_cast<char>(stream.peek()) != '#')
    {
        appendThrough(stream, result, '|');
    }

    result.push_back(static_cast<char>(stream.get()));

    return result;
}

std::string getString(std::istream& stream)
{
    std::string result;
    result.push_back(static_cast<char>(stream.get()));

    appendThrough(stream, result, '\'');
    while (static_cast<char>(stream.peek()) == '\'')
    {
        result.push_back(static_cast<char>(stream.get()));
        appendThrough(stream, result, '\'');
    }

    return result;
}

Grammar DatalogGrammarFactory::createDatalogGrammar()
{
    Grammar datalogGrammar;
    //datalogGrammar.addVariable(...);

    datalogGrammar.addTerminal(TokenType{
        std::string { "COMMENT" },
        std::bind(checkSequence, std::placeholders::_1, std::string{ "#|" }),
        getBlockComment }); 
    datalogGrammar.addTerminal(TokenType{
        std::string { "COMMENT" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '#'),
        getLineComment });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "STRING" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '\''),
        getString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "SCHEMES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Schemes" }),
        getIdentifier });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "FACTS" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Facts" }),
        getIdentifier });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "RULES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Rules" }),
        getIdentifier });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "QUERIES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Queries" }),
        getIdentifier });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "ID" },
        checkIdentifier,
        getIdentifier });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "COLON_DASH" },
        std::bind(checkSequence, std::placeholders::_1, ":-"),
        std::bind(getSequence, std::placeholders::_1, ":-") });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "COMMA" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ','),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "PERIOD" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '.'),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "Q_MARK" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '?'),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "L_PAREN" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '('),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "R_PAREN" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ')'),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "COLON" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ':'),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "MULTIPLY" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '*'),
        getSingleCharacterAsString });
    datalogGrammar.addTerminal(TokenType{
        std::string{ "ADD" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '+'),
        getSingleCharacterAsString });

    //datalogGrammar.addProduction(...);

    return datalogGrammar;
}