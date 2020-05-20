#include <catch2/catch.hpp>
#include <HtmlParser/Fixer.h>
#include <iostream>

#include "TokenizerTestHelper.h"
#include <tuple>

TEST_CASE("Fix token issues", "[Fixer]")
{
    std::vector<HtmlParser::Token> html_broken_no_structure {
        {HtmlParser::TokenType::TAG_OPEN, "div"},
        {HtmlParser::TokenType::TAG_CLOSE, "div"}
    };

    std::vector<HtmlParser::Token> html_broken_no_structure_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "html"},
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
        {HtmlParser::TokenType::DOCTYPE, "html"},
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
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"}, 
                    {HtmlParser::TokenType::TAG_CLOSE, "div"}, //<--- broken here
                {HtmlParser::TokenType::TAG_CLOSE, "p"},                
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_wrong_close_order_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"},
                    {HtmlParser::TokenType::TAG_CLOSE, "p"},
                {HtmlParser::TokenType::TAG_CLOSE, "div"}, 
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_close_tag{
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"}, // <--- broken here
                    {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_close_tag_FIXED{
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
            {HtmlParser::TokenType::TAG_OPEN, "head"},
            {HtmlParser::TokenType::TAG_CLOSE, "head"},
            {HtmlParser::TokenType::TAG_OPEN, "body"},
                {HtmlParser::TokenType::TAG_OPEN, "div"},
                    {HtmlParser::TokenType::TAG_OPEN, "p"}, 
                    {HtmlParser::TokenType::TAG_CLOSE, "p"}, 
                {HtmlParser::TokenType::TAG_CLOSE, "div"},
            {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    std::vector<HtmlParser::Token> html_broken_missing_start_tag{
        {HtmlParser::TokenType::DOCTYPE, "html"},
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
        {HtmlParser::TokenType::DOCTYPE, "html"},
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
        {HtmlParser::TokenType::DOCTYPE, "html"},
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
        {HtmlParser::TokenType::DOCTYPE, "html", 0},
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


    std::vector<std::tuple<std::string, std::vector<HtmlParser::Token>, std::vector<HtmlParser::Token>>> tests {
        /*
        {
            "No structure",
            html_broken_no_structure,
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
        //*/
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
   /*     {
            "Ensure correct weight is given",
            html_broken_no_weight,
            html_broken_no_weight_FIXED
        }*/
    };

    bool extra_debug{ true };

    for (auto[test_name, broken, fixed] : tests) {
        HtmlParser::Fixer fixer;

        if (extra_debug) {
            std::cout << "Test name: " << test_name << '\n';
            std::cout << "Before fix: \n";
            output_tokens(broken);
        }

        fixer.process(broken);

        if (extra_debug) {
            std::cout << "After fix: \n";
            output_tokens(broken);
            std::cout << "Expected fix: \n";
            output_tokens(fixed);
            std::cout << "------------------------------------------------------\n";
        }

        compare_values(broken, fixed, test_name);

    }

    
    //fixer.process(html_broken_no_structure);
    //fixer.process(html_broken_missing_doctype);
    //fixer.process(html_broken_wrong_close_order);
    //fixer.process(html_broken_missing_close_tag);
    //fixer.process(html_broken_missing_start_tag);

    //output_tokens(html_broken_missing_doctype);
    //output_tokens(html_broken_no_structure);
    //output_tokens(html_broken_wrong_close_order);
    //output_tokens(html_broken_missing_close_tag);
    //output_tokens(html_broken_missing_start_tag);
}
