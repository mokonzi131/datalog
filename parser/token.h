#pragma once

#include <istream>
#include <string>
#include <functional>

class TokenType
{
public:
    static TokenType Undefined;
    static TokenType Whitespace;
    static TokenType EndOfFile;

    TokenType(
        std::string name,
        std::function<bool(std::istream&)> matcher,
        std::function<std::string(std::istream&)> extractor);

    std::string name() const;
    bool matchesNext(std::istream& data) const;
    std::string extractNext(std::istream& data) const;

private:
    std::string _name;
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