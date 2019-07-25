#include "token.h"
#include <cctype>

TokenType::TokenType(
    std::string name,
    std::function<bool(std::ifstream&)> matcher,
    std::function<std::string(std::ifstream&)> extractor)
    : _name{ name }, _matcher{ matcher }, _extractor{ extractor } {}

TokenType TokenType::Undefined{
    std::string{ "UNDEFINED" },
    [](std::ifstream& stream) -> bool {
        auto value = stream.peek();
        if (value != std::char_traits<char>::eof())
            return true;

        return false;
    },
    [](std::ifstream& stream) -> std::string {
        int value = stream.get();
        if (!stream.eof())
            return std::string{ static_cast<char>(value) };

        return std::string{};
    } };

TokenType TokenType::EndOfFile{
    std::string{ "EOF" },
    [](std::ifstream& stream) -> bool {
        auto value = stream.peek();
        if (value == std::char_traits<char>::eof())
            return true;

        return false;
    },
    [](std::ifstream& stream) -> std::string {
        return std::string{};
    } };

TokenType TokenType::Whitespace{
    std::string{ "WHITESPACE" },
    [](std::ifstream& stream) -> bool {
        return std::isspace(stream.peek());
    },
    [](std::ifstream& stream) -> std::string {
        return std::string{ static_cast<char>(stream.get()) };
    } };

std::string TokenType::name() const { return _name; }

bool TokenType::matchesNext(std::ifstream& data) const { return _matcher(data); }

std::string TokenType::extractNext(std::ifstream& data) const { return _extractor(data); }


Token::Token(TokenType type, std::string value, size_t line)
    : _type{ type }, _value{ value }, _lineNumber{ line } {}

TokenType Token::type() const { return _type; }
std::string Token::value() const { return _value; }
size_t Token::line() const { return _lineNumber; }

std::ostream& operator<<(std::ostream& out, const Token& token)
{
    out << "(" << token.type().name() << ",\"" << token.value() << "\"," << token.line() << ")";
    return out;
}