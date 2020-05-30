#include <catch2/catch.hpp>
#include <HtmlParser/Document.h>
#include <HtmlParser/Element.h>

TEST_CASE("Docunent", "[Document]") {
    HtmlParser::Element html("html",
        HtmlParser::Element{"body",
    {
                HtmlParser::Element{HtmlParser::Element::Type::TEXT, "Hello document test"}, //
                HtmlParser::Element {HtmlParser::Element::Type::TAG_SELF_CLOSE, "img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")"},
                HtmlParser::Element { "div", R"(id="test" class="yellow zizzag test")", 
                    HtmlParser::Element{"div", R"(class="test")"}
                },
                HtmlParser::Element { "div", R"(class="test")" },
                HtmlParser::Element { "p", R"(class="test")" }
            }
        }
    );   

    HtmlParser::Document dom{ std::move(html), "html" };

    auto ele = dom.getElementById("test");

    REQUIRE(ele != nullptr);
    REQUIRE(ele->getElementName() == "div");
    REQUIRE(ele->hasAttributes());
    REQUIRE(ele->hasAttribute("class"));
    REQUIRE(ele->attr("class") == "yellow zizzag test");

    auto result = dom.getElementsByTagName("div");
    REQUIRE(!result.empty());
    REQUIRE(result.size() == 3);

    auto result2 = dom.getElementsByClassName("test");
    REQUIRE(!result2.empty());
    REQUIRE(result2.size() == 4);
}
