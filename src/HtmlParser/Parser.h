#pragma once
#include <vector>
#include <optional>
#include "Token.h"

namespace HtmlParser
{
    class Element;
    class Parser
    {
        std::size_t m_position{};
        std::vector<Token> m_tokens;
        std::vector<Element*> m_parent_list;
    public:
        void parse(const std::vector<Token>& tokens);

    private:
        Token* getToken();
        bool isCloseTagOfCurrentParent(const std::string& cs);
        void processAttributes(Element* ele, const Token* token);

    };
}