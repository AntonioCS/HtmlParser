#include <catch2/catch.hpp>
#include <HtmlParser/Reader.h>

namespace {
    const std::filesystem::path file{ std::string {"basic.html" } };
    const std::string file_data{
    R"(<!DOCTYPE html>
<html>
	<BODY>
		Hello
		<h1>My First Heading</h1>
		<p>My first paragraph.</p>
		<p>
			My first paragraph
			MultiLines!!123
		</p>
		<a 
		href=">https://www.w3schools.com?vecChar=T0xBIFRVRE8gQkVNPw=="
		
		
		>This is a link</a>
		<IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
		
		ddsadasasddasasdasd
	</body>
</html>)"
    };
}
/*
TEST_CASE("Read vecChar from file", "[reader]") {
    HtmlParser::Reader reader{ file };

    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    REQUIRE(reader.getString(14) == "!DOCTYPE html>");

    REQUIRE(reader.backPeek() == '<');
}
//*/

///*
TEST_CASE("Read vecChar from file in smaller chunks", "[reader]") {

    HtmlParser::Reader reader{ 5, file };

    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    //goes twice the size of the buffer
    REQUIRE(reader.getString(14) == "");

    REQUIRE(reader.backPeek() == '<');
}
//*/

TEST_CASE("Read vecChar from string", "[reader]")
{
    HtmlParser::Reader reader{ file_data };
    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    REQUIRE(reader.getString(14) == "!DOCTYPE html>");

    REQUIRE(reader.backPeek() == '<');

}
/*
TEST_CASE("Read vecChar from string in smaller chunks", "[reader]") {

    HtmlParser::Reader reader{ 5, file_data };

    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    //goes twice the size of the buffer
    REQUIRE(reader.getString(14) == "");

    REQUIRE(reader.backPeek() == '<');
}

*/

