#pragma once

#include <vector>
#include <string>

namespace HtmlParser::Util
{
    std::vector<std::string> split(const std::string& str, const char splitBy);
}
