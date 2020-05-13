#pragma once
#include <vector>
#include "Token.h"
#include "Document.h"

namespace HtmlParser
{
    class Element;
    class Parser
    {
    public:
        Document parse(const std::vector<Token>& tokens);
    };
}