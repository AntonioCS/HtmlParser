﻿#include "Element.h"

namespace HtmlParser
{
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
        AttributeStage stage{ AttributeStage::Invalid };
        char valueQuotes{ '"' };
        std::string key{};
        std::string value{};

        for (std::size_t i{}; i < attributesStr.size(); i++) {
            const auto c = attributesStr[i];

            switch (stage) {
                case AttributeStage::Invalid:
                    if (c != ' ') {
                        key = c;
                        stage = AttributeStage::Attr;
                    }
                break;
                case AttributeStage::Attr:
                {
                    switch (c) {
                        case '=':
                            stage = AttributeStage::ValueEntryPoint;
                        break;
                        case ' ':
                            stage = AttributeStage::Assign;
                        break;
                        default:
                            key += c;
                    }
                }
                break;
                case AttributeStage::ValueEntryPoint:
                {
                    if (c == '"' || c == '\'') {
                        valueQuotes = c;
                        stage = AttributeStage::ValueQuotes;
                    }
                    else if (c != ' ') {
                        value = c;
                        stage = AttributeStage::ValueNaked;
                    }
                }
                break;
                case AttributeStage::ValueNaked:
                {
                    if (c != ' ')
                        value += c;
                    else
                        stage = AttributeStage::Assign;
                }
                break;
                case AttributeStage::ValueQuotes:
                {
                    auto f = attributesStr.find(valueQuotes, i);
                    if (f == std::string::npos) {
                        f = attributesStr.size() - 1;
                    }

                    value = attributesStr.substr(i, f - i);
                    stage = AttributeStage::Assign;
                    i = f; //move i to the same position as the 'valueQuotes' char

                    if (i + 1 >= attributesStr.size())
                        i -= 2;
                }
                break;
                case AttributeStage::Assign:
                {
                    if (!key.empty()) {
                        m_attributes[key] = value;

                        key.clear();
                        value.clear();

                        stage = AttributeStage::Invalid;
                        i--;
                    }
                }
                break;
            }
        }
    }

    void Element::addChild(Element&& child)
    {
        child.setParent(this);
        m_children.push_back(child);
    }

    std::vector<Element>& Element::getChildren()
    {
        return m_children;
    }

    std::string_view Element::getElementName() const
    {
        return std::string_view{ m_name };
    }

    std::string Element::getInnerHtml() const
    {
        return m_inner_html;
    }

    void Element::setInnerHtml(const std::string& innerText)
    {
        m_inner_html = innerText;
    }

    bool Element::hasAttributes() const
    {
        return !m_attributes.empty();
    }

    bool Element::hasAttribute(const std::string& attr) const
    {
        return (m_attributes.find(attr) != m_attributes.end());
    }

    std::string Element::attr(const std::string& name)
    {
        if (hasAttribute(name))
            return m_attributes[name];

        return "";
    }

    void Element::attr(const std::string& name, std::string value)
    {
        m_attributes[name] = std::move(value);
    }

    bool Element::hasChildren() const
    {
        return !m_children.empty();
    }

    Element* Element::getFirstChild()
    {
        if (!m_children.empty())
            return &m_children.front();
        return nullptr;
    }

    Element* Element::getLastChild()
    {
        if (!m_children.empty())
            return &m_children.back();
        return nullptr;
    }

    Element* Element::getNthChild(std::size_t n)
    {
        if (!m_children.empty() && n < m_children.size())
            return &m_children.at(n);
        return nullptr;
    }

    std::size_t Element::getChildrenCount() const
    {
        return m_children.size();
    }

    std::size_t Element::getTotalChildrenCount() const
    {
        std::size_t total{};

        if (!m_children.empty()) {
            for (const auto& child : m_children) {
                total++;

                if (child.hasChildren())
                    total += child.getTotalChildrenCount();
            }
        }

        return total;
    }

    std::string Element::toString() const
    {
        if (m_name.empty()) {
            return m_inner_html;
        }

        std::string output{ "<" + m_name + toStringAttrList() + ">\n"};

        if (m_type == Type::NORMAL) {
            for (const auto& child : m_children) {
                output += "\t" + child.toString();
            }

            output += "\n</" + m_name + ">\n";
        }

        return output;
    }

    std::string Element::toStringAttrList() const
    {
        std::string output{};

        if (!m_attributes.empty()) {
            for (const auto& [key, value] : m_attributes) {
                output += ' ';
                output += key;
                output += "=\"";
                output += value;
                output += '"';
            }
        }

        return output;
    }

    std::string_view Element::getId() const
    {
        auto f = m_attributes.find("id");
        if (f != m_attributes.end()) {
            return std::string_view{ f->second };
        }

        return std::string_view{};
    }
}
