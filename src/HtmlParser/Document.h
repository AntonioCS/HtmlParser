#pragma once
#include "Element.h"

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
        std::vector<Element*> getElementsByTagName(std::string_view id);
        std::vector<Element*> getElementsByClassName(std::string_view id);
    private:
        Element* getElementById(std::vector<Element>& elements, std::string_view id);
    };
}