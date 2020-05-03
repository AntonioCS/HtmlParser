#pragma once
#include "Element.h"

namespace HtmlParser
{
    class Document
    {
        std::string m_doctype{};
        Element* m_root{nullptr};
    public:

        explicit Document(Element* root)
            : m_root(root)
        {
        }

        void setDoctype(std::string&& data)
        {
            m_doctype = data;
        }
    };
}