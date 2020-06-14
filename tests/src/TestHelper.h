#pragma once

#include <string>
#include <vector>
#include <HtmlParser/Token.h>
#include <filesystem>

void process_test(const std::string&, const std::vector<HtmlParser::Token>&, bool output_tokens_flag = false, bool output_cpp_tokens_flag = false);
void compare_values(const std::vector<HtmlParser::Token>&, const std::vector<HtmlParser::Token>&, std::string info = "");
void output_tokens(const std::vector<HtmlParser::Token>&);
void output_tokens(const std::vector<HtmlParser::Token>& tokens, std::vector<HtmlParser::Token>::iterator, std::vector<HtmlParser::Token>::iterator);
void output_cpp_tokens(
    const std::vector<HtmlParser::Token>&, 
    const std::string& var_name = "generated_tokens", 
    bool indent = true
);

std::string trim(const std::string&);
void printOutTokenTypeAsStr(const HtmlParser::TokenType&);
std::size_t totalProperTags(const std::vector<HtmlParser::Token>&);
void output_token(const HtmlParser::Token&);
void output_treeview(const std::vector<HtmlParser::Token>&);

class OutputCppTokens
{
    bool m_output_attr{ true };
    bool m_output_contents_of_text{ true };
    bool m_output_contents_of_code{ true };
    bool m_indent{ true };
    std::vector<HtmlParser::TokenType> m_ignored_types{};
    std::vector<std::string> m_ignored_tags{};
public:

    OutputCppTokens(
        bool attr, bool contents_of_text, bool contents_of_code, bool indent, 
        std::vector<HtmlParser::TokenType> ignored_types, 
        std::vector<std::string> ignored_tags)
        : m_output_attr(attr), m_output_contents_of_text(contents_of_text), m_output_contents_of_code(contents_of_code),
        m_indent(indent), m_ignored_types(std::move(ignored_types)), m_ignored_tags(std::move(ignored_tags))
    {

    }

    OutputCppTokens() = default;
 
    void output(const std::vector<HtmlParser::Token>& tokens, const std::string& var_name = "generated_tokens", std::filesystem::path* fp = nullptr);
};