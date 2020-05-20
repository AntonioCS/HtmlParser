#include <catch2/catch.hpp>
#include <HtmlParser/HtmlParser.h>
#include <HtmlParser/Document.h>
#include <HtmlParser/Element.h>
#include <iostream>
/*
TEST_CASE("Ensure correct processing", "[html_parser]") {

	const std::string expected_output ="<a id=\"mylink\" href=\"https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==\">\n\tThis is a link\n</a>\n";
	const std::string html_basic = R"(<!DOCTYPE html>
    <html>
	    <BODY>
		    Hello
		    <h1>My First Heading</h1>
		    <p>My first paragraph.</p>
		    <p>
			    My first paragraph
			    MultiLines!!123
		    </p>
		    <a id="mylink" href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==">This is a link</a>
		    <IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
		    
		    ddsadasasddasasdasd
	    </body>
    </html>)";

    HtmlParser::HtmlParser hp{};
	auto dom = hp.parse(html_basic);

	auto ele = dom.getElementById("mylink");
	REQUIRE(ele != nullptr);
	REQUIRE(ele->getInnerHtml() == expected_output);
}
*/

TEST_CASE("Ensure correct processing from file", "[html_parser]") {
	std::filesystem::path file{ std::string{"BigPage.html"} };
	HtmlParser::HtmlParser hp{file};
	auto dom = hp.parse();

	auto allLinks = dom.getElementById("primary")->getElementsByTagName("a");

	REQUIRE(!allLinks.empty());
}