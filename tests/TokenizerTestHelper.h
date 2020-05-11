#pragma once

#include <string>
#include <vector>
#include <HtmlParser/Token.h>

void process_test(const std::string&, const std::vector<HtmlParser::Token>&, bool output_tokens_flag = false, bool output_cpp_tokens_flag = false);
void compare_values(const std::vector<HtmlParser::Token>&, const std::vector<HtmlParser::Token>&);
void output_tokens(const std::vector<HtmlParser::Token>&);
void output_cpp_tokens(const std::vector<HtmlParser::Token>&, const std::string& var_name = "generated_tokens");
std::string trim(const std::string&);
void printOutTokenTypeAsStr(const HtmlParser::TokenType&);