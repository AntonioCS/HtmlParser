#pragma once

#include "Collection.h"

namespace HtmlParser {

    class Element;

    class ElementCollection : public Collection<Element*>
    {
    public:
        ElementCollection() = default;
    };
}