#include <catch2/catch.hpp>
#include <HtmlParser/Tokenizer.h>
#include <HtmlParser/Parser.h>
#include <string>
#include <iostream>

TEST_CASE("Parser", "[Parser Tokens]") {
    std::vector<HtmlParser::Token> tokens{
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "body"},
        {HtmlParser::TokenType::TEXT, "Hello"},
        {HtmlParser::TokenType::TAG_OPEN, "h1"},
        {HtmlParser::TokenType::TEXT, "My First Heading"},
        {HtmlParser::TokenType::TAG_CLOSE, "h1"},
        {HtmlParser::TokenType::TAG_OPEN, "p"},
        {HtmlParser::TokenType::TEXT, "My first paragraph."},
        {HtmlParser::TokenType::TAG_CLOSE, "p"},
        {HtmlParser::TokenType::TAG_OPEN, "p"},
        {HtmlParser::TokenType::TEXT, "\tMy first paragraph\n\tMultiLines!!123"},
        {HtmlParser::TokenType::TAG_CLOSE, "p"},
        {HtmlParser::TokenType::TAG_OPEN, "a", "href=\"https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==\""},
        {HtmlParser::TokenType::TEXT, "This is a link"},
        {HtmlParser::TokenType::TAG_CLOSE, "a"},
        {HtmlParser::TokenType::TAG_SELF_CLOSE, "img", "src=\"w3schools.jpg\" alt=\"W3Schools.com\" width=\"104\" height=\"142\""},
        {HtmlParser::TokenType::TEXT, "\t\t\n\t\tddsadasasddasasdasd"},
        {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };
}
