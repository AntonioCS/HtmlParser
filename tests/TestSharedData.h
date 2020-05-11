#pragma once

#include <vector>
#include <HtmlParser/Token.h>

static const std::vector<HtmlParser::Token> html_basic_expected_tokens{
      {HtmlParser::TokenType::DOCTYPE,"html"},
      {HtmlParser::TokenType::TAG_OPEN,"html"},
      {HtmlParser::TokenType::TAG_OPEN,"body"},
      {HtmlParser::TokenType::TEXT, R"(Hello)"},
      {HtmlParser::TokenType::TAG_OPEN,"h1"},
      {HtmlParser::TokenType::TEXT, R"(My First Heading)"},
      {HtmlParser::TokenType::TAG_CLOSE,"h1"},
      {HtmlParser::TokenType::TAG_OPEN,"p"},
      {HtmlParser::TokenType::TEXT, R"(My first paragraph.)"},
      {HtmlParser::TokenType::TAG_CLOSE,"p"},
      {HtmlParser::TokenType::TAG_OPEN,"p"},
      {HtmlParser::TokenType::TEXT, R"(
			    My first paragraph
			    MultiLines!!123
		    )"},
        {HtmlParser::TokenType::TAG_CLOSE,"p"},
        {HtmlParser::TokenType::TAG_OPEN,"a", R"(href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==")"},
        {HtmlParser::TokenType::TEXT, R"(This is a link)"},
        {HtmlParser::TokenType::TAG_CLOSE,"a"},
        {HtmlParser::TokenType::TAG_SELF_CLOSE,"img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")"},
        {HtmlParser::TokenType::TEXT, R"(ddsadasasddasasdasd)"},
        {HtmlParser::TokenType::TAG_CLOSE,"body"},
        {HtmlParser::TokenType::TAG_CLOSE,"html"}
};

static const std::vector<HtmlParser::Token> html_img_expected_tokens{
    {HtmlParser::TokenType::DOCTYPE, "html"},
    {HtmlParser::TokenType::TAG_OPEN, "html"},
    {HtmlParser::TokenType::TAG_OPEN, "body"},
    {HtmlParser::TokenType::TAG_SELF_CLOSE, "img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")"},
    {HtmlParser::TokenType::TAG_CLOSE, "body"},
    {HtmlParser::TokenType::TAG_CLOSE, "html"}
};

static const std::vector<HtmlParser::Token> html_comment_expected_tokens{
    {HtmlParser::TokenType::DOCTYPE, "html"},
    {HtmlParser::TokenType::TAG_OPEN, "html"},
    {HtmlParser::TokenType::TAG_OPEN, "body"},
    {HtmlParser::TokenType::TEXT, "text"},
    {HtmlParser::TokenType::TAG_CLOSE, "body"},
    {HtmlParser::TokenType::TAG_CLOSE, "html"}
};
