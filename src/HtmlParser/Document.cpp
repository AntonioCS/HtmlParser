#include "Document.h"

#include "Util.h"

namespace HtmlParser
{

    Element* Document::getElementById(std::string_view id)
    {
        return m_root.getElementById(id);
    }

    ElementCollection Document::getElementsByTagName(std::string_view tagName)
    {
        return m_root.getElementsByTagName(tagName);
    }

    ElementCollection Document::getElementsByClassName(std::string_view className)
    {
        return m_root.getElementsByClassName(className);
    }
    
}
