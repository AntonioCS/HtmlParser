#include "Document.h"

namespace HtmlParser
{

    Element* Document::getElementById(std::string_view id)
    {
        return getElementById(m_root.getChildren(), id);
    }

    Element* Document::getElementById(std::vector<Element>& elements, std::string_view id)
    {
        for (auto& element : elements) {
            if (element.getId() == id) {
                return &element;
            }

            if (element.hasChildren()) {
                if (auto res = getElementById(element.getChildren(), id);
                    res) {
                    return res;
                }
            }
        }

        return nullptr;
    }
}