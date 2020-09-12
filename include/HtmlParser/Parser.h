#pragma once
#include <vector>
#include "Token.h"
#include "Document.h"
#include <memory>

namespace HtmlParser
{
    class Element;
    class Parser
    {
        std::vector<Token> m_tokens{};
    public:
        Parser() = default;
        Parser(const std::vector<Token>& tokens);
        std::unique_ptr<Document> parse(const std::vector<Token>& tokens);
        std::unique_ptr<Document> parse() const;
    };
}
