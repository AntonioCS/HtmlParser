#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace HtmlParser
{
    class Element
    {
        Element* m_parent{ nullptr };
        std::string m_name{};
        std::vector<Element*> m_children{};
        std::unordered_map<std::string, std::string> m_attributes{};
        std::string m_inner_text;
    public:
        Element() = default;
        Element(const std::string name) : Element(nullptr, name) {}
        Element(Element *parent, const std::string name) : m_parent(parent), m_name(name) {}

        virtual ~Element();;
        [[nodiscard]] Element* getParent() const;
        void setParent(Element* p);
        void processAttributeString(const std::string& attributesStr);
        void addChild(Element* child);
        [[nodiscard]] const std::string& getElementName() const;


        std::string getInnerText() const;
        void setInnerText(const std::string& innerText);
    };
}