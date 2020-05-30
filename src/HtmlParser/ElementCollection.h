#pragma once

#include <functional>
#include <vector>
#include <HtmlParser/Collection.h>

namespace HtmlParser {

    class Element;

    class ElementCollection : public Collection<Element*>
    {
    public:
        ElementCollection() = default;
    };
}