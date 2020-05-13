#include "Document.h"
#include "Util.h"

namespace HtmlParser
{

    Element* Document::getElementById(std::string_view id)
    {
        return getElementById(m_root.getChildren(), id);
    }

    std::vector<Element*> Document::getElementsByTagName(std::string_view tagName)
    {
        return getElementsByCondition(m_root.getChildren(), [tagName](Element& ele)
        {
                return ele.tagName == tagName;
        });
    }

    std::vector<Element*> Document::getElementsByClassName(std::string_view className)
    {
        return getElementsByCondition(m_root.getChildren(), [className](Element& ele)
            {
                auto eleClasses = ele.attr("class");

                if (!eleClasses.empty()) {
                    auto res = Util::split(eleClasses, ' ');

                    for (const auto& cName : res) {
                        if (cName == className)
                            return true;
                    }
                }

                return false;
            });
    }

    std::vector<Element*> Document::getElementsByCondition(std::vector<Element>& elements, const std::function<bool(Element&)>& func)
    {
        std::vector<Element*> res{};

        for (auto& element : elements) {
            if (func(element))
                res.push_back(&element);

            if (element.hasChildren()) {
                auto res2 = getElementsByCondition(element.getChildren(), func);

                if (!res2.empty()) {
                    res.insert(res.end(), res2.begin(), res2.end());
                }
            }
        }

        return res;
    }

    Element* Document::getElementById(std::vector<Element>& elements, std::string_view id)
    {
        for (auto& element : elements) {
            if (element.getId() == id) {
                return &element;
            }

            if (element.hasChildren()) {
                auto res = getElementById(element.getChildren(), id);

                if (res) {
                    return res;
                }
            }
        }

        return nullptr;
    }
}
