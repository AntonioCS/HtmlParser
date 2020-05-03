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
        TEXT
    };

    struct Token
    {
        TokenType type;
        std::string tag{};
        std::string attributes{};

        Token(const TokenType type, const std::string t, const std::string attributes);
        Token(const TokenType type, const std::string t);
    };

}