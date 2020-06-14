#include <catch2/catch.hpp>
#include <HtmlParser/Fixer.h>
#include <iostream>
#include <HtmlParser/Tokenizer.h>
#include "TestHelper.h"
#include <tuple>

#include "HtmlParser/TokenCollection.h"

TEST_CASE("Fix token issues", "[Fixer]")
{
    std::vector<HtmlParser::Token> html_broken_no_structure {
        {HtmlParser::TokenType::TAG_OPEN, "div"},
        {HtmlParser::TokenType::TAG_CLOSE, "div"}
    };

    std::vector<HtmlParser::Token> html_broken_no_structure_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"},
    };

    std::vector<HtmlParser::Token> html_broken_missing_doctype {
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"}, 
                {HtmlParser::TokenType::TAG_OPEN, "p"},
                {HtmlParser::TokenType::TAG_CLOSE, "p"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_doctype_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
                {HtmlParser::TokenType::TAG_OPEN, "p"},
                {HtmlParser::TokenType::TAG_CLOSE, "p"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_wrong_close_order{
        //{HtmlParser::TokenType::DOCTYPE, "html"},
        //{HtmlParser::TokenType::TAG_OPEN, "html"},
        //    {HtmlParser::TokenType::TAG_OPEN, "head"},
        //    {HtmlParser::TokenType::TAG_CLOSE, "head"},
        //    {HtmlParser::TokenType::TAG_OPEN, "body"},
            {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_OPEN, "p"},
                    {HtmlParser::TokenType::TEXT, "Text!"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"}, //<--- broken here
            {HtmlParser::TokenType::TAG_CLOSE, "p"},
        //    {HtmlParser::TokenType::TAG_CLOSE, "body"},
        //{HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_wrong_close_order_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"},
                        {HtmlParser::TokenType::TEXT, "Text!"},
                    {HtmlParser::TokenType::TAG_CLOSE, "p"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"}, 
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_close_tag{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"}, // <--- broken here
                        {HtmlParser::TokenType::TEXT, "Text!"},
                    {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_close_tag_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"},
                        {HtmlParser::TokenType::TEXT, "Text!"},
                    {HtmlParser::TokenType::TAG_CLOSE, "p"}, 
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_start_tag{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"}, // <--- broken here
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"}, 
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_start_tag_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_no_weight {
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TEXT, "Text"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_no_weight_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "doctype", "html", 0},
        {HtmlParser::TokenType::TAG_OPEN, "html", 0},
            {HtmlParser::TokenType::TAG_OPEN, "head", 10},
            {HtmlParser::TokenType::TAG_CLOSE, "head", 10},
            {HtmlParser::TokenType::TAG_OPEN, "body", 10},
                {HtmlParser::TokenType::TAG_OPEN, "div", 20},
                    {HtmlParser::TokenType::TEXT, "Text", 30},
                {HtmlParser::TokenType::TAG_CLOSE, "div", 20},
            {HtmlParser::TokenType::TAG_CLOSE, "body", 10},
        {HtmlParser::TokenType::TAG_CLOSE, "html", 0}
    };

    const std::vector<HtmlParser::Token> html_broken_tags_missing {
        {HtmlParser::TokenType::DOCTYPE,"doctype", R"XX(html)XX", 0, 0},
        {HtmlParser::TokenType::TAG_OPEN,"html", 0},
            {HtmlParser::TokenType::TAG_OPEN,"head", 10},
            {HtmlParser::TokenType::TAG_CLOSE,"head", 10},
            {HtmlParser::TokenType::TAG_OPEN,"body", 10},
                {HtmlParser::TokenType::TAG_OPEN,"div",20},
                {HtmlParser::TokenType::TAG_CLOSE,"div", 20},
                {HtmlParser::TokenType::TAG_OPEN,"center", 20},
                    {HtmlParser::TokenType::TAG_OPEN,"h3", 30},
                        {HtmlParser::TokenType::TAG_OPEN,"strong", 40},
                            {HtmlParser::TokenType::TEXT, R"XX(Advertisement)XX", 50},
                            {HtmlParser::TokenType::TAG_OPEN,"center", 50},
                                {HtmlParser::TokenType::TAG_OPEN,"div", R"XX(style="width: 336px; height: 280px; position: relative;")XX", 60},
                                    {HtmlParser::TokenType::TAG_OPEN,"div", 70},
                                        {HtmlParser::TokenType::TAG_OPEN,"div", R"XX(style="background-image: url()")XX", 80},
                                            {HtmlParser::TokenType::TEXT, R"XX(BLA)XX", 90},
                                            {HtmlParser::TokenType::TAG_SELF_CLOSE,"img", 90},
                                        {HtmlParser::TokenType::TAG_CLOSE,"div", 80},
                                    {HtmlParser::TokenType::TAG_CLOSE,"center", 70}, // <-- Broken here
                                {HtmlParser::TokenType::TAG_CLOSE,"h3", 60},
                            {HtmlParser::TokenType::TAG_CLOSE,"strong", 50},
                        {HtmlParser::TokenType::TAG_CLOSE,"center", 40}, // <-- Broken here
                    {HtmlParser::TokenType::TAG_CLOSE,"body", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"html", 20}
    };

    const std::vector<HtmlParser::Token> html_broken_tags_missing_FIXED {
        {HtmlParser::TokenType::DOCTYPE,"doctype", R"XX(html)XX", 0, 0},
        {HtmlParser::TokenType::TAG_OPEN,"html", 0, 437},
          {HtmlParser::TokenType::TAG_OPEN,"head", 10, 402},
          {HtmlParser::TokenType::TAG_CLOSE,"head", 10, 402},
          {HtmlParser::TokenType::TAG_OPEN,"body", 10, 430},
            {HtmlParser::TokenType::TAG_OPEN,"div", 20, 323},
            {HtmlParser::TokenType::TAG_CLOSE,"div", 20, 323},
            {HtmlParser::TokenType::TAG_OPEN,"center", 20, 641},
              {HtmlParser::TokenType::TAG_OPEN,"h3", 30, 155},
                {HtmlParser::TokenType::TAG_OPEN,"strong", 40, 669},
                  {HtmlParser::TokenType::TEXT, R"XX(Advertisement)XX", 50, 0},
                  {HtmlParser::TokenType::TAG_OPEN,"center", 50, 641},
                    {HtmlParser::TokenType::TAG_OPEN,"div", R"XX(style="width: 336px; height: 280px; position: relative;")XX", 60, 323},
                      {HtmlParser::TokenType::TAG_OPEN,"div", 70, 323},
                        {HtmlParser::TokenType::TAG_OPEN,"div", R"XX(style="background-image: url()")XX", 80, 323},
                          {HtmlParser::TokenType::TEXT, R"XX(BLA)XX", 90, 0},
                          {HtmlParser::TokenType::TAG_SELF_CLOSE,"img", 90, 317},
                        {HtmlParser::TokenType::TAG_CLOSE,"div", 80, 323},
                      {HtmlParser::TokenType::TAG_CLOSE,"div", 70, 323},
                    {HtmlParser::TokenType::TAG_CLOSE,"div", 60, 323},
                  {HtmlParser::TokenType::TAG_CLOSE,"center", 50, 641},
                {HtmlParser::TokenType::TAG_CLOSE,"strong", 40, 669},
              {HtmlParser::TokenType::TAG_CLOSE,"h3", 30, 155},
            {HtmlParser::TokenType::TAG_CLOSE,"center", 20, 641},
          {HtmlParser::TokenType::TAG_CLOSE,"body", 10, 430},
        {HtmlParser::TokenType::TAG_CLOSE,"html", 0, 437}
    };




#if 0
    std::string html_broken = "";
    std::string correct_html = "";

    HtmlParser::Tokenizer t{html_broken};
    //HtmlParser::Tokenizer t{ correct_html };
    t.tokenize();
    auto res = t.getTokens();
    std::cout << "Tokens: " << res.size() << '\n';

    OutputCppTokens o{};
    o.output(res);
    //output_cpp_tokens(res);
    exit(-1);
#endif


    std::vector<std::tuple<std::string, HtmlParser::TokenCollection, std::vector<HtmlParser::Token>>> tests {
        {
            "No structure",
            {html_broken_no_structure},
            html_broken_no_structure_FIXED
        },
        {
            "Missing doctype",
            html_broken_missing_doctype,
            html_broken_missing_doctype_FIXED
        },
        {
            "Wrong close order",
            html_broken_wrong_close_order,
            html_broken_wrong_close_order_FIXED
        },
       {
            "Missing close tag",
            html_broken_missing_close_tag,
            html_broken_missing_close_tag_FIXED
        },
        {
            "Missing start tag",
            html_broken_missing_start_tag,
            html_broken_missing_start_tag_FIXED
        },
        {
            "Ensure correct weight is given",
            html_broken_no_weight,
            html_broken_no_weight_FIXED
        },
        {
            "Broken html, multiple tags missing",
            html_broken_tags_missing,
            html_broken_tags_missing_FIXED
        }
    };
    
    HtmlParser::TokenCollection tc{};

    for (auto[test_name, broken, fixed] : tests) {
        HtmlParser::Fixer fixer;
#if 0
        std::cout << "Test name: " << test_name << '\n';
        std::cout << "------------------------------------------------------\n";
        std::cout << "Before fix: \n";
        output_tokens(broken.getAllTokens());
        std::cout << "------------------------------------------------------\n\n";
#endif

        fixer.process(broken);

#if 0
        std::cout << "------------------------------------------------------\n";
        std::cout << "After fix: \n";
        output_tokens(broken.getAllTokens());
        std::cout << "Expected fix: \n";
        output_tokens(fixed);
        std::cout << "------------------------------------------------------\n";
#endif

        compare_values(broken.getAllTokens(), fixed, test_name);
    }
}
