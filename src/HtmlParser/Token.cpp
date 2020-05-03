#include "Token.h"

namespace HtmlParser
{
    Token::Token(const TokenType type, const std::string t, const std::string attributes) :
        type(type),
        tag(t),
        attributes(attributes)
    {
    }

    Token::Token(const TokenType type, const std::string t) : Token(type, t, "")
    {
    }
}