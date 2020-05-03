#include "Util.h"

namespace HtmlParser::Util
{
    std::vector<std::string> split(const std::string& str, const char splitBy) {
        std::size_t last_pos{ 0 };
        std::size_t pos{};
        std::vector<std::string> res{};

        while ((pos = str.find(splitBy, last_pos)) != std::string::npos) {
            res.push_back(str.substr(last_pos, pos - last_pos));
            last_pos = pos + 1;
        }

        //get the last string
        res.push_back(str.substr(last_pos, str.size() - last_pos));

        return res;
    }
}