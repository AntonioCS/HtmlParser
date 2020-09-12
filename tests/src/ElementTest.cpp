#include <catch2/catch.hpp>
#include <HtmlParser/Element.h>
#include <iostream>

/*
TEST_CASE("Ensure correct creation of element", "[Element]") {
    HtmlParser::Element ele{ "div", R"(id="test" class="yellow zizzag")" };

    REQUIRE(ele.getElementName() == "div");
    //REQUIRE(ele.tagName == "div");
    REQUIRE(ele.hasAttributes() == true);
    REQUIRE(ele.getId() == "test");
    REQUIRE(ele.hasAttribute("class") == true);
    REQUIRE(ele.attr("class") == "yellow zizzag");

    REQUIRE(ele.toStringAttrList() == R"( id="test" class="yellow zizzag")");
}

TEST_CASE("Ensure correct element registers children", "[Element]") {
    HtmlParser::Element ele{ "div", R"(id="test" class="yellow zizzag")",
        HtmlParser::Element{"p",
            HtmlParser::Element{HtmlParser::Element::Type::TEXT, "My text"}
        }
    };

    REQUIRE(ele.hasChildren() == true);
    REQUIRE(ele.getChildrenCount() == 1);
    REQUIRE(ele.getFirstChild()->getElementName() == "p");

    REQUIRE(ele.getFirstChild()->toString() == "<p>\n\tMy text\n</p>\n");

    //std::cout << ele.toString();
}

///*
TEST_CASE("Ensure getElementsByTagName works", "[Element]") {
    HtmlParser::Element ele{ "div", R"(id="test" class="yellow zizzag")",
        {
        	HtmlParser::Element{"p",
				HtmlParser::Element{ HtmlParser::Element::Type::TEXT, "My text"}
			},
            HtmlParser::Element{"p",
				HtmlParser::Element{ HtmlParser::Element::Type::TEXT, "My text 2"}
			}
        }
    };

    REQUIRE(ele.hasChildren() == true);

    auto res = ele.getElementsByTagName("p");
    REQUIRE(res.empty() == false);
}
//*/