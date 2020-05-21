#include <catch2/catch.hpp>
#include <HtmlParser/TokenCollection.h>
#include "TestHelper.h"


TEST_CASE("Ensure correct creation", "[TokenCollection]") {
    HtmlParser::TokenCollection tokenCollection{};

    tokenCollection
        .add(HtmlParser::TokenType::DOCTYPE, "html")
        ->add(HtmlParser::TokenType::TAG_OPEN, "html")
            ->add(HtmlParser::TokenType::TAG_OPEN, "head")
            ->add(HtmlParser::TokenType::TAG_CLOSE, "head")
            ->add(HtmlParser::TokenType::TAG_OPEN, "body")
                ->add(HtmlParser::TokenType::TEXT, R"(Hello)")
                ->add(HtmlParser::TokenType::TAG_OPEN, "h1")
                    ->add(HtmlParser::TokenType::TEXT, R"(My First Heading)")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "h1")
                ->add(HtmlParser::TokenType::TAG_OPEN, "p")
                    ->add(HtmlParser::TokenType::TEXT, R"(My first paragraph.)")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "p")
                ->add(HtmlParser::TokenType::TAG_OPEN, "p")
                    ->add(HtmlParser::TokenType::TEXT, "Text")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "p")
                ->add(HtmlParser::TokenType::TAG_OPEN, "a", R"(href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==")")
                    ->add(HtmlParser::TokenType::TEXT, R"(This is a link)")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "a")
                ->add(HtmlParser::TokenType::TAG_SELF_CLOSE, "img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")")
                ->add(HtmlParser::TokenType::TEXT, R"(ddsadasasddasasdasd)")
            ->add(HtmlParser::TokenType::TAG_CLOSE, "body")
        ->add(HtmlParser::TokenType::TAG_CLOSE, "html")
    ;

    const std::vector<HtmlParser::Token> expected_tokens{
        {HtmlParser::TokenType::DOCTYPE,"html", 0},
        {HtmlParser::TokenType::TAG_OPEN,"html", 0},
            {HtmlParser::TokenType::TAG_OPEN,"head", 10},
            {HtmlParser::TokenType::TAG_CLOSE,"head", 10},
            {HtmlParser::TokenType::TAG_OPEN,"body", 10},
                {HtmlParser::TokenType::TEXT, R"(Hello)", 20},
                {HtmlParser::TokenType::TAG_OPEN,"h1", 20},
                    {HtmlParser::TokenType::TEXT, R"(My First Heading)", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"h1", 20},
                {HtmlParser::TokenType::TAG_OPEN,"p", 20},
                    {HtmlParser::TokenType::TEXT, R"(My first paragraph.)", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"p", 20},
                {HtmlParser::TokenType::TAG_OPEN,"p", 20},
                    {HtmlParser::TokenType::TEXT, R"(Text)", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"p", 20},
                {HtmlParser::TokenType::TAG_OPEN,"a", R"(href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==")", 20},
                    {HtmlParser::TokenType::TEXT, R"(This is a link)", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"a", 20},
                {HtmlParser::TokenType::TAG_SELF_CLOSE,"img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")", 20},
                {HtmlParser::TokenType::TEXT, R"(ddsadasasddasasdasd)", 20},
            {HtmlParser::TokenType::TAG_CLOSE,"body", 10},
        {HtmlParser::TokenType::TAG_CLOSE,"html", 0}
    };

    auto res = tokenCollection.getAllTokens();

    compare_values(res, expected_tokens, "Collection and raw vector are equal");
}

TEST_CASE("Ensure findTag functions properly", "[TokenCollection]") {
    HtmlParser::TokenCollection tokenCollection{};

    tokenCollection
        .add(HtmlParser::TokenType::DOCTYPE, "html")
        ->add(HtmlParser::TokenType::TAG_OPEN, "html")
            ->add(HtmlParser::TokenType::TAG_OPEN, "body")
                ->add(HtmlParser::TokenType::TEXT, R"(Hello)")
                ->add(HtmlParser::TokenType::TAG_OPEN, "h1")
                    ->add(HtmlParser::TokenType::TEXT, R"(My First Heading)")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "h1")                
            ->add(HtmlParser::TokenType::TAG_CLOSE, "body")
        ->add(HtmlParser::TokenType::TAG_CLOSE, "html");

    auto res = tokenCollection.findTag("body");

    REQUIRE(res != tokenCollection.end());
    REQUIRE(res->type == HtmlParser::TokenType::TAG_OPEN);
    REQUIRE(res->tag == "body");

    res = tokenCollection.findTag(HtmlParser::TokenType::TAG_CLOSE, "body");
    REQUIRE(res != tokenCollection.end());
    REQUIRE(res->type == HtmlParser::TokenType::TAG_CLOSE);
    REQUIRE(res->tag == "body");
}

TEST_CASE("Insert tag", "[TokenCollection]") {
    HtmlParser::TokenCollection tokenCollection{};

    tokenCollection
        .add(HtmlParser::TokenType::DOCTYPE, "doctype","html")
        ->add(HtmlParser::TokenType::TAG_OPEN, "html")
            ->add(HtmlParser::TokenType::TAG_OPEN, "body")
                ->add(HtmlParser::TokenType::TEXT, R"(Hello)")
                ->add(HtmlParser::TokenType::TAG_OPEN, "h1")
                    ->add(HtmlParser::TokenType::TEXT, R"(My First Heading)")
                ->add(HtmlParser::TokenType::TAG_CLOSE, "h1")                
            ->add(HtmlParser::TokenType::TAG_CLOSE, "body")
        ->add(HtmlParser::TokenType::TAG_CLOSE, "html")
    ;

    const std::vector<HtmlParser::Token> expected_tokens{
        {HtmlParser::TokenType::DOCTYPE,"doctype", "html", 0},
        {HtmlParser::TokenType::TAG_OPEN,"html", 0},
            {HtmlParser::TokenType::TAG_OPEN,"head", 10},
            {HtmlParser::TokenType::TAG_CLOSE,"head", 10},
            {HtmlParser::TokenType::TAG_OPEN,"body", 10},
                {HtmlParser::TokenType::TEXT, R"(Hello)", 20},
                {HtmlParser::TokenType::TAG_OPEN,"h1", 20},
                    {HtmlParser::TokenType::TEXT, R"(My First Heading)", 30},
                {HtmlParser::TokenType::TAG_CLOSE,"h1", 20},        
            {HtmlParser::TokenType::TAG_CLOSE,"body", 10},
        {HtmlParser::TokenType::TAG_CLOSE,"html", 0}
    };

    auto res = tokenCollection.findTag("head");

#if 0
    output_tokens(tokenCollection.getAllTokens());
#endif

    if (res == tokenCollection.end()) {

        res = tokenCollection.findTag("html");
        INFO("Found html tag");
        REQUIRE(res != tokenCollection.end());
    
        tokenCollection.insert(++res, {
            {HtmlParser::TokenType::TAG_OPEN,"head"},
            {HtmlParser::TokenType::TAG_CLOSE,"head"},
        });
        
 #if 0
        output_tokens(tokenCollection.getAllTokens());
#endif
        compare_values(tokenCollection.getAllTokens(), expected_tokens, "Inject head");
    }
}

