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
        std::size_t level{};
        std::size_t code{};

        static const std::size_t level_amount{ 10 };

        Token(const TokenType type, const std::string& t)
            : Token(type, t, "") {}
        Token(const TokenType type, const std::string& t, const std::string& attr)
            : Token(type, t, attr, 0) {}
        Token(const TokenType type, const std::string& t, std::size_t wght)
            : Token(type, t,"", wght){}
        Token(const TokenType type, const std::string& t, std::size_t wght, std::size_t level)
            : Token(type, t, "", wght, level) {}
        Token(const TokenType type, const std::string& t, const std::string& attr, std::size_t wght)
            : type(type), tag(t), attributes(attr), level(wght)
        {
            if (type == TokenType::TAG_OPEN || type == TokenType::TAG_CLOSE || type == TokenType::TAG_SELF_CLOSE) {
                for (const auto& c : tag) {
                    code += static_cast<std::size_t>(c);
                }
            }
        }

        Token(const TokenType type, const std::string& t, const std::string& attr, std::size_t wght, std::size_t code)
            : type(type), tag(t), attributes(attr), level(wght), code(code) {}
    };

}