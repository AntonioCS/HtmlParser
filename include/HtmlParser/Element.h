#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ElementCollection.h"

namespace HtmlParser
{
    class Element
    {
    public:
        enum class Type
        {
            TEXT,
            TAG_SELF_CLOSE,
            TAG
        };

        const std::string tagName;
    private:
        Type m_type{ Type::TAG };
        Element* m_parent{ nullptr };
        std::string m_name{};
        std::vector<Element> m_children{};
        std::unordered_map<std::string, std::string> m_attributes{};
        std::string m_inner_html{};

        enum class AttributeStage {
            Attr,
            ValueEntryPoint,
            ValueQuotes,
            ValueNaked,
            Assign,
            Invalid
        };
    public:
        Element() = default;

        Element(const std::string& name) : Element(Type::TAG, nullptr, name) {}

        Element(const std::string& name, const std::string& attributes) : Element(Type::TAG, nullptr, name, attributes) {}

        Element(Element* parent, const std::string& name) : Element(Type::TAG, parent, name) {}

        Element(Element* parent, const std::string& name, const std::string& attributes) : Element(Type::TAG, parent, name, attributes) {}

        Element(Type type, const std::string& nameOrData) : Element(type, nullptr, nameOrData) {}

        Element(Type type, Element* parent, const std::string& nameOrData) : Element(type, parent, nameOrData, "") {}

        Element(Type type, const std::string& name, const std::string& attributes) : Element(type, nullptr, name, attributes) {}

        Element(Type type, Element* parent, const std::string& nameOrData, const std::string& attributes) : tagName(type == Type::TEXT ? "" : nameOrData), m_type(type), m_parent(parent), m_name(nameOrData){
            if (!attributes.empty())
                processAttributeString(attributes);

            if (type == Type::TEXT) {
                m_name.clear();
                m_attributes.clear();

                m_inner_html = nameOrData;
            }
        }

        //Following constructors don't need type as it will always be the default of NORMAL
        Element(const std::string& name, Element&& child) : Element(nullptr, name, "", std::move(child)) {}

        Element(const std::string& name, const std::string& attributes, Element&& child) : Element(nullptr, name, attributes, std::move(child)) {}

        Element(Element* parent, const std::string& name, const std::string& attributes, Element&& child) : Element(parent, name, attributes, std::vector<Element>{ child }) {}

        Element(const std::string& name, std::vector<Element>&& children) : Element(nullptr, name, "", std::move(children)) {}

        Element(const std::string& name, const std::string& attributes, std::vector<Element>&& children) : Element(nullptr, name, attributes, std::move(children)) {}

        Element(Element* parent, const std::string& name, const std::string& attributes, std::vector<Element>&& children)
            : tagName(name), m_parent(parent), m_name(name)
        {
            if (!attributes.empty())
                processAttributeString(attributes);

            for (auto&& child : children) {
                addChild(std::move(child));
            }
        }

        virtual ~Element() = default;

        [[nodiscard]] Element* getParent() const;
        void setParent(Element* p);
        void addChild(Element&& child);
        [[nodiscard]] std::vector<Element>& getChildren();
        [[nodiscard]] std::string_view getElementName() const;

        [[nodiscard]] std::string getInnerHtml() const;
        void setInnerHtml(const std::string& innerText);

        [[nodiscard]] bool hasAttributes() const;
        [[nodiscard]] bool hasAttribute(const std::string&) const;
        [[nodiscard]] bool hasClass(std::string_view) const;

        [[nodiscard]] std::string attr(const std::string&) const;
        //[[nodiscard]] std::string attr(std::string_view;
        void attr(const std::string&, std::string);
        [[nodiscard]] bool hasChildren() const;
        [[nodiscard]] Element* getFirstChild();
        [[nodiscard]] Element* getLastChild();
        [[nodiscard]] Element* getNthChild(std::size_t n);
        [[nodiscard]] std::size_t getChildrenCount() const;
        [[nodiscard]] std::size_t getTotalChildrenCount() const;
        //@todo: improve output
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::string toStringAttrList() const;
        [[nodiscard]] std::string_view getId() const;

        Element* getElementById(std::string_view id);
        ElementCollection getElementsByTagName(std::string_view tagName);
        ElementCollection getElementsByClassName(std::string_view className);

    private:
        void processAttributeString(const std::string& attributesStr);
        [[nodiscard]] bool isTag() const { return m_type == Type::TAG || m_type == Type::TAG_SELF_CLOSE; }
    };
}
