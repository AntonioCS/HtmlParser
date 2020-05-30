#pragma once

#include "Element.h"
#include "ElementCollection.h"

namespace HtmlParser
{
    class Document
    {
        Element m_root;
        std::string m_doctype{};

    public:

        Document(Element&& root, std::string doctype)
            : m_root(root), m_doctype(std::move(doctype))
        {
        }

        Element& html()
        {
            return m_root;
        }

        Element* getElementById(std::string_view id);
        ElementCollection getElementsByTagName(std::string_view tagName);
        ElementCollection getElementsByClassName(std::string_view className);
    };
}
