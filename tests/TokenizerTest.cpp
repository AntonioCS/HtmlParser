#include <catch2/catch.hpp>
#include <string>
#include "TestSharedData.h"
#include "TokenizerTestHelper.h"

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