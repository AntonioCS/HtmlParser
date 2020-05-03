#include <catch2/catch.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <HtmlParser/Tokenizer.h>
//#include <algorithm> //std::equal -- Did not work

void process_test(const std::string&, const std::vector<HtmlParser::Token>&, bool output_tokens_flag = false, bool output_cpp_tokens_flag = false);
void compare_values(const std::vector<HtmlParser::Token>&, const std::vector<HtmlParser::Token>&);
void output_tokens(const std::vector<HtmlParser::Token>&);
void output_cpp_tokens(const std::vector<HtmlParser::Token>&, const std::string& var_name = "generated_tokens");
std::string trim(const std::string&);
void printOutTokenTypeAsStr(const HtmlParser::TokenType&);


//All TEXT elements are trimed
TEST_CASE("Tokenizer", "[Ensure correct generation of tokens based on given html]") {
    std::string html_basic = R"(<!DOCTYPE html>
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
    </html>
    )";

    const std::vector<HtmlParser::Token> html_basic_expected_tokens{
        {HtmlParser::TokenType::DOCTYPE,"html"},
        {HtmlParser::TokenType::TAG_OPEN,"html"},
        {HtmlParser::TokenType::TAG_OPEN,"body"},
        {HtmlParser::TokenType::TEXT, R"(Hello)"},
        {HtmlParser::TokenType::TAG_OPEN,"h1"},
        {HtmlParser::TokenType::TEXT, R"(My First Heading)"},
        {HtmlParser::TokenType::TAG_CLOSE,"h1"},
        {HtmlParser::TokenType::TAG_OPEN,"p"},
        {HtmlParser::TokenType::TEXT, R"(My first paragraph.)"},
        {HtmlParser::TokenType::TAG_CLOSE,"p"},
        {HtmlParser::TokenType::TAG_OPEN,"p"},
        {HtmlParser::TokenType::TEXT, R"(
			    My first paragraph
			    MultiLines!!123
		    )"},
        {HtmlParser::TokenType::TAG_CLOSE,"p"},
        {HtmlParser::TokenType::TAG_OPEN,"a", R"(href="https://www.w3schools.com?data=T0xBIFRVRE8gQkVNPw==")"},
        {HtmlParser::TokenType::TEXT, R"(This is a link)"},
        {HtmlParser::TokenType::TAG_CLOSE,"a"},
        {HtmlParser::TokenType::TAG_SELF_CLOSE,"img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")"},
        {HtmlParser::TokenType::TEXT, R"(ddsadasasddasasdasd)"},
        {HtmlParser::TokenType::TAG_CLOSE,"body"},
        {HtmlParser::TokenType::TAG_CLOSE,"html"}
    };


    std::string html_img = R"(<!DOCTYPE html>
        <html>
            <BODY>
            <IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>
            </body>
        </html>
    )";
    const std::vector<HtmlParser::Token> html_img_expected_tokens{
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "body"},
        {HtmlParser::TokenType::TAG_SELF_CLOSE, "img", R"(src="w3schools.jpg" alt="W3Schools.com" width="104" height="142")"},
        {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    process_test(html_basic, html_basic_expected_tokens);
    //process_test(html_img, html_img_expected_tokens);
}


TEST_CASE("Tokenizer2", "[HTML with spaces]") {
    std::string html_comment = R"(<!DOCTYPE html>
        <html>
            <BODY>
            text
            <!--<IMG src="w3schools.jpg" alt="W3Schools.com" width="104" height="142"/>-->
            </body>
        </html>
    )";


    const std::vector<HtmlParser::Token> html_comment_expected_tokens{    
        {HtmlParser::TokenType::DOCTYPE, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "html"},
        {HtmlParser::TokenType::TAG_OPEN, "body"},
        {HtmlParser::TokenType::TEXT, "text"},
        {HtmlParser::TokenType::TAG_CLOSE, "body"},
        {HtmlParser::TokenType::TAG_CLOSE, "html"}
    };

    //process_test(html_comment, html_comment_expected_tokens);
}



void process_test(const std::string& html, const std::vector<HtmlParser::Token>& expected, bool output_tokens_flag, bool output_cpp_tokens_flag)
{
    HtmlParser::Tokenizer t{ html };
    //t.tokenize();
    t.tokenize2();

    auto result = t.getTokens();

    if (output_cpp_tokens_flag)
        output_cpp_tokens(result);

    if (output_tokens_flag) {
        std::cout << "-------------------------- Result --------------------------\n";
        output_tokens(result);
        std::cout << "-------------------------- Expected -------------------------- \n";
        output_tokens(expected);
    }

    compare_values(result, expected);
}

void compare_values(const std::vector<HtmlParser::Token>& result, const std::vector<HtmlParser::Token>& expected)
{

    //output_cpp_tokens(result);
    REQUIRE(result.size() == expected.size());

    SECTION("the values are the same") {

        for (std::size_t i{}; i < result.size(); i++) {

            auto b = (
                result[i].type == expected[i].type &&
                trim(result[i].tag) == trim(expected[i].tag) &&
                result[i].attributes == expected[i].attributes
               );

            if (!b) {
                std::cout << "Token type of result: ";
                printOutTokenTypeAsStr(result[i].type);
                std::cout << "\nToken type of expected: ";
                printOutTokenTypeAsStr(expected[i].type);

                std::cout << "\nTags compare: " << result[i].tag.compare(expected[i].tag) << '\n';
                std::cout << "Tags size: " << result[i].tag.size() << " - " << expected[i].tag.size() << '\n';
                std::cout << "Tags: \n'" << result[i].tag << "'\n'" << expected[i].tag << "'\n";

                std::cout << "Attributes of result: " << result[i].attributes << '\n';
                std::cout << "Attributes of expected " << expected[i].attributes << '\n';
            }

            REQUIRE(b);
        }
    }    
}

void output_tokens(const std::vector<HtmlParser::Token>& tokens)
{
    for (const auto& token : tokens) {
        std::cout << "Type: ";
        switch (token.type) {
            case HtmlParser::TokenType::DOCTYPE:
                std::cout << "DOCTYPE";
                break;
            case HtmlParser::TokenType::TEXT:
                std::cout << "TEXT";
                break;
            case HtmlParser::TokenType::TAG_OPEN:
                std::cout << "TAG OPEN";
                break;
            case HtmlParser::TokenType::TAG_CLOSE:
                std::cout << "TAG CLOSE";
                break;
            case HtmlParser::TokenType::TAG_SELF_CLOSE:
                std::cout << "TAG SELF CLOSE";
                break;
        }
        std::cout << " - Tag: '" << token.tag << '\''; 

        if (!token.attributes.empty())
            std::cout << " - Attributes: " << token.attributes;

        std::cout << '\n';
    }
}


void output_cpp_tokens(const std::vector<HtmlParser::Token>& tokens, const std::string& var_name)
{
    std::cout << "const std::vector<HtmlParser::Token> " << var_name << " {\n";

    for (std::size_t i{}; i < tokens.size(); i++) {
        auto token = tokens[i];       
        std::cout << '{';
        
        printOutTokenTypeAsStr(token.type);

        if (token.type == HtmlParser::TokenType::TEXT) {            
            std::cout << ", R\"(" << token.tag << ")\"";
        }
        else {
            std::cout << ",\"" << token.tag << '"';
        }

        if (!token.attributes.empty()) {            
            std::cout << ", R\"(" << token.attributes << ")\"";
        }

        std::cout << ((i+1 < tokens.size()) ? "},\n" : "}\n");
    }
    std::cout << "};\n";
}

std::string trim(const std::string& str)
{
    auto trim{ str };

    std::size_t left{};
    for (; left < str.size(); left++) {
        auto c = str[left];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            break;
        }
    }

    if (left > 0) trim = str.substr(left);

    std::size_t right{ trim.size() - 1 };
    for (; right > 0; right--) {
        auto c = trim[right];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            right++;
            break;
        }
    }

    return trim.substr(0, right);
}

void printOutTokenTypeAsStr(const HtmlParser::TokenType& tokenType)
{
    switch (tokenType) {
    case HtmlParser::TokenType::DOCTYPE:
        std::cout << "HtmlParser::TokenType::DOCTYPE";
        break;
    case HtmlParser::TokenType::TEXT:
        std::cout << "HtmlParser::TokenType::TEXT";
        break;
    case HtmlParser::TokenType::TAG_OPEN:
        std::cout << "HtmlParser::TokenType::TAG_OPEN";
        break;
    case HtmlParser::TokenType::TAG_CLOSE:
        std::cout << "HtmlParser::TokenType::TAG_CLOSE";
        break;
    case HtmlParser::TokenType::TAG_SELF_CLOSE:
        std::cout << "HtmlParser::TokenType::TAG_SELF_CLOSE";
        break;
    }
}
