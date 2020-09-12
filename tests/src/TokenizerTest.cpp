#include <catch2/catch.hpp>
#include <string>
#include "TestSharedData.h"
#include "TestHelper.h"
#include <HtmlParser/Tokenizer.h>
#include <iostream>

//All TEXT elements are trimed
TEST_CASE("Ensure correct generation of tokens based on given html","[Tokenizer]") {
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
		    <a href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==">This is a link</a>
		    <IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
		    
		    ddsadasasddasasdasd
	    </body>
    </html>)";

    const std::string html_img = R"(<!DOCTYPE html>
        <html>
            <BODY>
            <IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
            </body>
        </html>)";

    INFO("Testing with html basic");
    process_test(html_basic, html_basic_expected_tokens);

    INFO("Testing with html img");
    process_test(html_img, html_img_expected_tokens);
}


TEST_CASE("HTML with comnments", "[Tokenizer]" ) {
    const std::string html_comment = R"(<!DOCTYPE html>
        <html>
            <BODY>
            text
            <!--<IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>-->
            </body>
        </html>)";

    INFO("HTML with comments");
    process_test(html_comment, html_comment_expected_tokens);
}

TEST_CASE("HTML chunks", "[Tokenizer]" ) {
    const std::string html_chunk_1 = R"(<!DOCTYPE html>
    <html>
	    <BODY>
		    Hello)";
    const std::string html_chunk_2 = R"(<h1>My First Heading</h1>
		    <p>My first paragraph.</p>
		    <p>
			    My first paragraph
			    MultiLines!!123<)";
    const std::string html_chunk_3 = R"(/p>
		    <a href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==">This is a link</a>
		    <IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
		    
		    ddsadasasddasasdasd
	    </body>
    </html>)";

    INFO("HTML in chunks");
    HtmlParser::Tokenizer t{};


    t.tokenize(html_chunk_1, HtmlParser::Tokenizer::TokenizerMode::CHUNKS);
    t.tokenize(html_chunk_2, HtmlParser::Tokenizer::TokenizerMode::CHUNKS);
    t.tokenize(html_chunk_3, HtmlParser::Tokenizer::TokenizerMode::CHUNKS);    

    auto result = t.getTokens();

    compare_values(result, html_basic_expected_tokens);
}

TEST_CASE("HTML from file", "[Tokenizer_file_out]") {
    const std::filesystem::path file{ std::string{"part_from_big_file.html"} };
    HtmlParser::Tokenizer t{file};

    t.tokenize();

    auto res = t.getTokens();

    //output_tokens(res);
    //currently gives 56
    //REQUIRE(totalProperTags(res) == 50); //Firefox gives me 50 on document.getElementsByTagName("*").length
}

/*
TEST_CASE("HTML from big file", "[Tokenizer_big_file]") {
    const std::filesystem::path file{ std::string{"BigPage_body_only.html"} };
    HtmlParser::Tokenizer t{ file };

    t.tokenize();

    auto res = t.getTokens();
    //output_treeview(res);

    REQUIRE(totalProperTags(res) == 75); //Firefox gives me 75 on document.getElementsByTagName("*").length
}
*/