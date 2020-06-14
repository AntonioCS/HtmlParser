#include <iostream>
#include <catch2/catch.hpp>
#include "TestHelper.h"
#include "HtmlParser/Tokenizer.h"


void process_test(const std::string& html, const std::vector<HtmlParser::Token>& expected, bool output_tokens_flag, bool output_cpp_tokens_flag)
{
    HtmlParser::Tokenizer t{ html };
    t.tokenize();

    auto result = t.getTokens();

    if (output_cpp_tokens_flag)
        output_cpp_tokens(result);

    if (output_tokens_flag) {
        std::cout << "-------------------- Tokens Output Start ------------------\n";
        std::cout << "-------------------------- Result --------------------------\n";
        output_tokens(result);
        std::cout << "-------------------------- Expected ------------------------\n";
        output_tokens(expected);
        std::cout << "-------------------- Tokens Output End ---------------------\n\n";
    }

    compare_values(result, expected);
}

void compare_values(const std::vector<HtmlParser::Token>& result, const std::vector<HtmlParser::Token>& expected, std::string info)
{

    /*std::cout << "Cpp tokens:\n";
    output_cpp_tokens(result);
    std::cout << "Cpp tokens end\n";*/

    INFO(info);
    
    REQUIRE(result.size() == expected.size());

    SECTION("the values are the same") {

        for (std::size_t i{}; i < result.size(); i++) {

            auto b = (
                result[i].type == expected[i].type &&
                trim(result[i].tag) == trim(expected[i].tag) &&
                result[i].attributes == expected[i].attributes
            );

            if (expected[i].level != 0 && b) {
                b = b && result[i].level == expected[i].level;
            }

            if (!b) {
                std::cout << "---- Comparision breakdown ---- \n";
                std::cout << "Token type of result: ";
                printOutTokenTypeAsStr(result[i].type);
                std::cout << "\nToken type of expected: ";
                printOutTokenTypeAsStr(expected[i].type);
                std::cout << '\n';
                if (result[i].type != expected[i].type) std::cout << "TYPES DIFFER\n";

                std::cout << "Tags compare: " << result[i].tag.compare(expected[i].tag) << '\n';
                std::cout << "Tags size: " << result[i].tag.size() << " - " << expected[i].tag.size() << '\n';
                std::cout << "Tags: \n'" << result[i].tag << "'\n'" << expected[i].tag << "'\n";
                if (trim(result[i].tag) != trim(expected[i].tag)) std::cout << "TAGS DIFFER\n";

                std::cout << "Attributes of result: '" << result[i].attributes << "'\n";
                std::cout << "Attributes of expected: '" << expected[i].attributes << "'\n";
                if (result[i].attributes != expected[i].attributes) std::cout << "ATTRIBUTES DIFFER\n";

                if (!info.empty()) 
                    std::cout << "Test msg: " << info << '\n';
                
            }

            REQUIRE(b);
        }
    }
}

void output_token(const HtmlParser::Token& token)
{
    std::string name{ "Tag" };
    std::cout << "Type: ";

    switch (token.type) {
    case HtmlParser::TokenType::DOCTYPE:
        std::cout << "DOCTYPE";
        //name = "Attributes";
        break;
    case HtmlParser::TokenType::TEXT:
        std::cout << "TEXT";
        name = "Text";
        break;
    case HtmlParser::TokenType::TAG_OPEN:
        std::cout << "TAG OPEN";
        break;
    case HtmlParser::TokenType::TAG_CLOSE:
        std::cout << "TAG CLOSE";
        break;
    case HtmlParser::TokenType::TAG_SELF_CLOSE:
        std::cout << "TAG SELF CLOSE";
        break;
    case HtmlParser::TokenType::CODE:
        std::cout << "CODE";
        name = "Code";
        break;
    }
    std::cout << "; Level: " << token.level
            << "; Code: " << token.code
            << " - " << name << ": '" << token.tag << '\'';

    if (!token.attributes.empty())
        std::cout << " - Attributes: " << token.attributes;

    std::cout << '\n';
}

void output_tokens(const std::vector<HtmlParser::Token>& tokens)
{
    std::cout << "Size: " << tokens.size() << '\n';

    for (const auto& token : tokens) {
        output_token(token);
    }
}

void output_tokens(const std::vector<HtmlParser::Token>& tokens, std::vector<HtmlParser::Token>::iterator start, std::vector<HtmlParser::Token>::iterator end)
{
    std::cout << "Size: " << std::distance(start, end) << '\n';

    while(start != end) {
        output_token(*start++);
    }
}


void output_cpp_tokens(const std::vector<HtmlParser::Token>& tokens, const std::string& var_name, bool indent)
{
    std::cout << "const std::vector<HtmlParser::Token> " << var_name << " {\n";

    for (std::size_t i{}; i < tokens.size(); i++) {
        auto token = tokens[i];

        if (indent && token.level + 10 > 0) {
            std::cout << "  ";
            for (std::size_t s{ 0 }; s < ((token.level + 10) / 10); s++)
                std::cout << "  ";
        }

        std::cout << '{';

        printOutTokenTypeAsStr(token.type);

        if (token.type == HtmlParser::TokenType::TEXT || token.type == HtmlParser::TokenType::CODE) {
            std::cout << ", R\"XX(" << token.tag << ")XX\"";
        }
        else {
            std::cout << ", \"" << token.tag << '"';
        }

        if (!token.attributes.empty()) {
            std::cout << ", R\"XX(" << token.attributes << ")XX\"";
        }

        std::cout << ", " << token.level << ", " << token.code;

        std::cout << ((i + 1 < tokens.size()) ? "},\n" : "}\n");
    }
    std::cout << "};\n";    
}

std::string trim(const std::string& str)
{
    auto trim{ str };

    std::size_t left{};
    for (; left < str.size(); left++) {
        auto c = str[left];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            break;
        }
    }

    if (left > 0) trim = str.substr(left);

    std::size_t right{ trim.size() - 1 };
    for (; right > 0; right--) {
        auto c = trim[right];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            right++;
            break;
        }
    }

    return trim.substr(0, right);
}

void printOutTokenTypeAsStr(const HtmlParser::TokenType& tokenType)
{
    switch (tokenType) {
        case HtmlParser::TokenType::DOCTYPE:
            std::cout << "HtmlParser::TokenType::DOCTYPE";
            break;
        case HtmlParser::TokenType::TEXT:
            std::cout << "HtmlParser::TokenType::TEXT";
            break;
        case HtmlParser::TokenType::TAG_OPEN:
            std::cout << "HtmlParser::TokenType::TAG_OPEN";
            break;
        case HtmlParser::TokenType::TAG_CLOSE:
            std::cout << "HtmlParser::TokenType::TAG_CLOSE";
            break;
        case HtmlParser::TokenType::TAG_SELF_CLOSE:
            std::cout << "HtmlParser::TokenType::TAG_SELF_CLOSE";
            break;
        case HtmlParser::TokenType::CODE:
            std::cout << "HtmlParser::TokenType::CODE";
            break;
        default:
            std::cout << "/* Unknown */";
    }
}


std::size_t totalProperTags(const std::vector<HtmlParser::Token>& tokens)
{
    std::size_t total{};

    for (const auto& ele : tokens) {
        if (ele.type != HtmlParser::TokenType::TAG_CLOSE
            && ele.type != HtmlParser::TokenType::TEXT
            //&& ele.type != HtmlParser::TokenType::CODE
            && ele.type != HtmlParser::TokenType::DOCTYPE
            )
            total++;
    }

    return total;
}

void output_treeview(const std::vector<HtmlParser::Token>& tokens)
{
    std::size_t tabs_num{ 0 };
    std::vector<std::string> marker{};
    for (const auto& ele : tokens) {
        std::string tabs(tabs_num, '\t');
        std::string tag = ele.tag;
        bool p{ true };

        switch (ele.type) {
            case HtmlParser::TokenType::DOCTYPE:
                continue;
            case HtmlParser::TokenType::TAG_OPEN:
                marker.push_back(tag + " - " + std::to_string(tabs_num));
                tag = marker.back();
                tabs_num++;
            break;
            case HtmlParser::TokenType::TAG_CLOSE:
                tag = marker.back();
                tag.insert(0, 1, '/');
                marker.pop_back();
                tabs.pop_back();
                tabs_num--;
            break;
            case HtmlParser::TokenType::TEXT:
            case HtmlParser::TokenType::CODE:
                p = false;
            break;
            default:;
        }

        if (p) {
            std::cout << tabs << tag << '\n';
        }
    }
}


void OutputCppTokens::output(const std::vector<HtmlParser::Token>& tokens, const std::string& var_name, std::filesystem::path* fp)
{
    std::ofstream fout;
    std::streambuf* backup{};

    if (fp) {
        fout.open(fp->c_str());
        if (fout.is_open()) {
            backup = std::cout.rdbuf();
            std::cout.rdbuf(fout.rdbuf());
        } else {
            std::cout << "Unable to open file\n";
            exit(-1);
        }
    }

    std::cout << "const std::vector<HtmlParser::Token> " << var_name << " {\n";

    for (std::size_t i{}; i < tokens.size(); i++) {
        auto token = tokens[i];

        if ((!m_ignored_types.empty() && std::find(m_ignored_types.begin(), m_ignored_types.end(), token.type) != m_ignored_types.end()) ||
            (!m_ignored_tags.empty() && std::find(m_ignored_tags.begin(), m_ignored_tags.end(), token.tag) != m_ignored_tags.end())) 
        {
            continue;
        }

        if (m_indent && token.level+10 > 0) {
            std::cout << "  ";
            for (std::size_t n{0}; n < ((token.level+10) / 10); n++)
                std::cout << "  ";
        }

        std::cout << '{';

        printOutTokenTypeAsStr(token.type);

        if (token.type == HtmlParser::TokenType::TEXT || token.type == HtmlParser::TokenType::CODE) {
            if (token.type == HtmlParser::TokenType::TEXT && !m_output_contents_of_text || 
                token.type == HtmlParser::TokenType::CODE && !m_output_contents_of_code) std::cout << ", \"\"";
            else std::cout << ", R\"XX(" << token.tag << ")XX\"";
        }
        else {
            std::cout << ", \"" << token.tag << '"';
        }

        if (!token.attributes.empty()) {
            if (m_output_attr) std::cout << ", R\"XX(" << token.attributes << ")XX\"";
            else std::cout << ", \"\"";
        }

        std::cout << ", " << token.level << ", " << token.code;

        std::cout << ((i + 1 < tokens.size()) ? "},\n" : "}\n");
    }
    std::cout << "};\n";

    if (fp && fout.is_open()) {
        std::cout.rdbuf(backup);
    }
}