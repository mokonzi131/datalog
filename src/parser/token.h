#pragma once

#include <istream>
#include <string>
#include <functional>

class Variable
{
public:
    Variable(std::string name);

    std::string name() const;

private:
    std::string _name;
};

class TokenType : public Variable
{
public:
    static TokenType Undefined;
    static TokenType Whitespace;
    static TokenType EndOfFile;

    TokenType(
        std::string name,
        std::function<bool(std::istream&)> matcher,
        std::function<std::string(std::istream&)> extractor);

    bool matchesNext(std::istream& data) const;
    std::string extractNext(std::istream& data) const;

private:
    std::function<bool(std::istream&)> _matcher;
    std::function<std::string(std::istream&)> _extractor;
};

class Token
{
public:
    Token(TokenType type, std::string value, size_t line);

    TokenType type() const;
    std::string value() const;
    size_t line() const;

private:
    TokenType _type;
    std::string _value;
    size_t _lineNumber;
};

std::ostream& operator<<(std::ostream& out, const Token& token);