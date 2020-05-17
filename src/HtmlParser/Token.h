#pragma once

#include <string>

namespace HtmlParser
{
    enum class TokenType
    {
        DOCTYPE,
        TAG_SELF_CLOSE,
        TAG_OPEN,
        TAG_CLOSE,
        TEXT,
        CODE
    };

    struct Token
    {
        TokenType type;
        std::string tag{};
        std::string attributes{};
        std::size_t weight{};

        Token(const TokenType type, const std::string t) : Token(type, t, "") {}
        Token(const TokenType type, const std::string t, const std::string& attr) : Token(type, t, attr, 0) {}
        Token(const TokenType type, const std::string& t, const std::string& attr, std::size_t wght) : type(type), tag(t), attributes(attr), weight(wght){}
    };

}