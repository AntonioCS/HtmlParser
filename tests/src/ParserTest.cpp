#include <catch2/catch.hpp>
#include <HtmlParser/Tokenizer.h>
#include "TestSharedData.h"
#include <HtmlParser/Parser.h>


TEST_CASE("Produce document", "[Parser]") {    
    HtmlParser::Parser parser;

    auto doc = parser.parse(html_basic_expected_tokens);
    //auto doc = parser.parse(html_img_expected_tokens);

    REQUIRE(doc->html().getTotalChildrenCount() == 12);
    //std::cout << "HTML:\n" << doc.html()->toString();
}
