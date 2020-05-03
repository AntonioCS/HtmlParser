#include "Element.h"
#include "Util.h"

namespace HtmlParser
{
    Element::~Element()
    {
        for (auto child : m_children) {
            free(child);
        }
    }

    Element* Element::getParent() const
    {
        return m_parent;
    }

    void Element::setParent(Element* p)
    {
        m_parent = p;
    }

    void Element::processAttributeString(const std::string& attributesStr)
    {
        auto allBySpace = Util::split(attributesStr, ' ');
        for (const auto& keyValue : allBySpace) {
            auto res = Util::split(keyValue, '=');
            //m_attributes.
        }
    }

    void Element::addChild(Element* child)
    {
        child->setParent(this);
        m_children.push_back(child);
    }

    const std::string& Element::getElementName() const
    {
        return m_name;
    }

    std::string Element::getInnerText() const
    {
        return m_inner_text;
    }

    void Element::setInnerText(const std::string& innerText)
    {
        m_inner_text = innerText;
    }
}
