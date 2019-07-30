#include "grammar.h"
#include <functional>
#include <string>

Grammar::Grammar(Variable startSymbol)
    : _startSymbol{ startSymbol }
{
    _variables.push_back(startSymbol);
}

void Grammar::addTerminal(TokenType tokenType) { _terminals.push_back(tokenType); }

void Grammar::addProduction(std::pair<Variable, std::vector<Variable>> production)
{
    _productions.push_back(production);
    for (const Variable& variable : production.second)
    {
        auto it = std::find_if(std::cbegin(_variables), std::cend(_variables), [&variable](const Variable& candidate) { 
            return variable.name() == candidate.name();
        });
        if (it == std::cend(_variables))
        {
            _variables.push_back(variable);
        }
    }
}

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
    Variable datalogProgram{ std::string{ "DATALOG" } };
    Variable scheme{ std::string{ "SCHEME" } };
    Variable schemeList{ std::string{ "SCHEME_LIST" } };
    Variable idList{ std::string{ "ID_LIST" } };
    Variable fact{ std::string{ "FACT" } };
    Variable factList{ std::string{ "FACT_LIST" } };
    Variable rule{ std::string{ "RULE" } };
    Variable ruleList{ std::string{ "RULE_LIST" } };
    Variable headPredicate{ std::string{ "HEAD_PREDICATE" } };
    Variable predicate{ std::string{ "PREDICATE" } };
    Variable predicateList{ std::string{ "PREDICATE_LIST" } };
    Variable parameter{ std::string{ "PARAMETER" } };
    Variable parameterList{ std::string{ "PARAMETER_LIST" } };
    Variable expression{ std::string{ "EXPRESSION" } };
    Variable operatorr{ std::string{ "OPERATOR" } };
    Variable query{ std::string{ "QUERY" } };
    Variable queryList{ std::string{ "QUERY_LIST" } };
    Variable stringList{ std::string{ "STRING_LIST" } };

    TokenType COMMENT{ std::string { "COMMENT" },
        std::bind(checkSequence, std::placeholders::_1, std::string{ "#|" }),
        getBlockComment };
    TokenType COMMENT2{ std::string { "COMMENT" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '#'),
        getLineComment };
    TokenType STRING{ std::string{ "STRING" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '\''),
        getString };
    TokenType SCHEMES{ std::string{ "SCHEMES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Schemes" }),
        getIdentifier };
    TokenType FACTS{ std::string{ "FACTS" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Facts" }),
        getIdentifier };
    TokenType RULES{ std::string{ "RULES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Rules" }),
        getIdentifier };
    TokenType QUERIES{ std::string{ "QUERIES" },
        std::bind(checkKeyword, std::placeholders::_1, std::string{ "Queries" }),
        getIdentifier };
    TokenType ID{ std::string{ "ID" },
        checkIdentifier,
        getIdentifier };
    TokenType COLDASH{ std::string{ "COLON_DASH" },
        std::bind(checkSequence, std::placeholders::_1, ":-"),
        std::bind(getSequence, std::placeholders::_1, ":-") };
    TokenType COMMA{ std::string{ "COMMA" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ','),
        getSingleCharacterAsString };
    TokenType PERIOD{ std::string{ "PERIOD" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '.'),
        getSingleCharacterAsString };
    TokenType QMARK{ std::string{ "Q_MARK" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '?'),
        getSingleCharacterAsString };
    TokenType LPAREN{ std::string{ "L_PAREN" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '('),
        getSingleCharacterAsString };
    TokenType RPAREN{ std::string{ "R_PAREN" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ')'),
        getSingleCharacterAsString };
    TokenType COLON{ std::string{ "COLON" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, ':'),
        getSingleCharacterAsString };
    TokenType MULTIPLY{ std::string{ "MULTIPLY" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '*'),
        getSingleCharacterAsString };
    TokenType ADD{ std::string{ "ADD" },
        std::bind(checkSpecificCharacter, std::placeholders::_1, '+'),
        getSingleCharacterAsString };

    Grammar datalogGrammar{ datalogProgram };

    datalogGrammar.addTerminal(COMMENT); 
    datalogGrammar.addTerminal(COMMENT2);
    datalogGrammar.addTerminal(STRING);
    datalogGrammar.addTerminal(SCHEMES);
    datalogGrammar.addTerminal(FACTS);
    datalogGrammar.addTerminal(RULES);
    datalogGrammar.addTerminal(QUERIES);
    datalogGrammar.addTerminal(ID);
    datalogGrammar.addTerminal(COLDASH);
    datalogGrammar.addTerminal(COMMA);
    datalogGrammar.addTerminal(PERIOD);
    datalogGrammar.addTerminal(QMARK);
    datalogGrammar.addTerminal(LPAREN);
    datalogGrammar.addTerminal(RPAREN);
    datalogGrammar.addTerminal(COLON);
    datalogGrammar.addTerminal(MULTIPLY);
    datalogGrammar.addTerminal(ADD);

    using VarList = std::vector<Variable>;
    std::vector<Variable> EPSILON;
    datalogGrammar.addProduction(std::make_pair(datalogProgram,
        VarList{ { SCHEMES, COLON, scheme, schemeList, FACTS, COLON, factList,
            RULES, COLON, ruleList, QUERIES, COLON, query, queryList } }));
    datalogGrammar.addProduction(std::make_pair(scheme,
        VarList{ { ID, LPAREN, ID, idList, RPAREN } }));
    datalogGrammar.addProduction(std::make_pair(schemeList, VarList{ { scheme, schemeList } }));
    datalogGrammar.addProduction(std::make_pair(schemeList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(idList, VarList{ { COMMA, ID, idList } }));
    datalogGrammar.addProduction(std::make_pair(idList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(fact,
        VarList{ { ID, LPAREN, STRING, stringList, RPAREN, PERIOD } }));
    datalogGrammar.addProduction(std::make_pair(factList, VarList{ { fact, factList } }));
    datalogGrammar.addProduction(std::make_pair(factList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(rule,
        VarList{ { headPredicate, COLDASH, predicate, predicateList, PERIOD } }));
    datalogGrammar.addProduction(std::make_pair(ruleList, VarList{ { rule, ruleList } }));
    datalogGrammar.addProduction(std::make_pair(ruleList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(headPredicate,
        VarList{ { ID, LPAREN, ID, idList, RPAREN } }));
    datalogGrammar.addProduction(std::make_pair(predicate,
        VarList{ { ID, LPAREN, parameter, parameterList, RPAREN } }));
    datalogGrammar.addProduction(std::make_pair(predicateList, 
        VarList{ { COMMA, predicate, predicateList } }));
    datalogGrammar.addProduction(std::make_pair(predicateList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(parameter, VarList{ { STRING } }));
    datalogGrammar.addProduction(std::make_pair(parameter, VarList{ { ID } }));
    datalogGrammar.addProduction(std::make_pair(parameter, VarList{ { expression } }));
    datalogGrammar.addProduction(std::make_pair(parameterList, VarList{ { COMMA, parameter, parameterList } }));
    datalogGrammar.addProduction(std::make_pair(parameterList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(expression,
        VarList{ { LPAREN, parameter, operatorr, parameter, RPAREN } }));
    datalogGrammar.addProduction(std::make_pair(operatorr, VarList{ { ADD } }));
    datalogGrammar.addProduction(std::make_pair(operatorr, VarList{ { MULTIPLY } }));
    datalogGrammar.addProduction(std::make_pair(query, VarList{ { predicate, QMARK } }));
    datalogGrammar.addProduction(std::make_pair(queryList, VarList{ { query, queryList } }));
    datalogGrammar.addProduction(std::make_pair(queryList, EPSILON));
    datalogGrammar.addProduction(std::make_pair(stringList, VarList{ { COMMA, STRING, stringList } }));
    datalogGrammar.addProduction(std::make_pair(stringList, EPSILON));

    return datalogGrammar;
}