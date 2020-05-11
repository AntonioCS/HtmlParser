#include <catch2/catch.hpp>
#include <HtmlParser/Element.h>


TEST_CASE("Ensure correct creation of element","[Element]") {
    HtmlParser::Element ele{ "div", R"(id="test" class="yellow zizzag")" };

    REQUIRE(ele.getElementName() == "div");
    REQUIRE(ele.hasAttributes() == true);
    REQUIRE(ele.getId() == "test");
    REQUIRE(ele.hasAttribute("class") == true);
    REQUIRE(ele.attr("class") == "yellow zizzag");
}