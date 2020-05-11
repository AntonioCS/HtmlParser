#include <catch2/catch.hpp>
#include <HtmlParser/Reader.h>


static std::filesystem::path file{ std::string {"basic.html" } };
///*
TEST_CASE("Read data from file", "[reader]") {
    Reader reader{ file };

    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    REQUIRE(reader.getString(14) == "!DOCTYPE html>");

    REQUIRE(reader.backPeek() == '<');
}
//*/

TEST_CASE("Read data from file in smaller chunks", "[reader]") {

    Reader reader{ 5, file };

    REQUIRE(reader.getChar() == '<');

    REQUIRE(reader.getString(9) == "<!DOCTYPE");
    reader.incPosition();

    //goes twice the size of the buffer
    REQUIRE(reader.getString(14) == "");

    REQUIRE(reader.backPeek() == '<');
}
//*/



    //std::string_view data{ "Lorem ipsum dolor sit amet, consectetur adipiscing" };
    //Reader r{ 1, data};

    /*
    auto buf = r.getBufferSize();
    std::size_t i{ 0 };

    char c;
    //test read
    while ((c = r.getChar()) != '\0') {
        std::cout << c;
        r.incPosition();
    }
    */

    /* Test peek
    std::cout << r.getChar() << '\n';
    std::cout << r.peek() << '\n';
    std::cout << r.getChar() << '\n';
    r.incPosition();
    std::cout << r.getChar() << '\n';
    //*/

    //std::cout << r.getChar() << '\n';
    //r.incPosition();
    //std::cout << r.getString(2) << '\n';
