#include <filesystem>
#include <utility>
#include <array>
#include <functional>
#include <tuple>
#include <HtmlParser/Fixer.h>
#include <HtmlParser/TokenCollection.h>


#define DEBUG_FIXER 0
#if DEBUG_FIXER
    #include "../tests/TestHelper.h"
    #include <iostream>
#endif

namespace HtmlParser
{
    void Fixer::process(TokenCollection& tokenCollection)
    {
        if (!tokenCollection.empty()) {
            ensureProperStructure(tokenCollection);
            analyseAndFix(tokenCollection);
        }
    }

    void Fixer::ensureProperStructure(TokenCollection& tokenCollection)
    {
#if DEBUG_FIXER
        std::cout << "Ensure Proper structure before:\n";
        output_tokens(tokenCollection.getAllTokens());
#endif

        std::array<std::tuple<TokenType, std::string, std::function<void(void)>>, 6> check_for_existance {
            std::make_tuple(TokenType::DOCTYPE, "doctype", [&tokenCollection]()
            {
                tokenCollection.insert(tokenCollection.begin(), { TokenType::DOCTYPE,"doctype", "html" });
            }),
            std::make_tuple(TokenType::TAG_OPEN, "html", [&tokenCollection]()
            {
                tokenCollection.insert(1, {TokenType::TAG_OPEN,"html"});
            }),
            std::make_tuple(TokenType::TAG_CLOSE, "html", [&tokenCollection]()
            {
                tokenCollection.insert(tokenCollection.end(), {TokenType::TAG_CLOSE,"html"});
            }),
            std::make_tuple(TokenType::TAG_OPEN, "head", [&tokenCollection]()
            {
                tokenCollection.insert(
                    tokenCollection.findTag("html") + 1, 
                    { {TokenType::TAG_OPEN,"head"}, {TokenType::TAG_CLOSE,"head"} }
                );
            }),
            std::make_tuple(TokenType::TAG_OPEN, "body", [&tokenCollection]()
            {
                tokenCollection.insert(
                    tokenCollection.findTag(TokenType::TAG_CLOSE,"head") + 1,
                    {TokenType::TAG_OPEN,"body"}
                );
            }),
            std::make_tuple(TokenType::TAG_CLOSE, "body", [&tokenCollection]()
            {
                tokenCollection.insert(
                    tokenCollection.findTag(TokenType::TAG_CLOSE,"html", TokenCollection::FindType::END),
                    {TokenType::TAG_CLOSE,"body"}
                );
            }),
        };

        for (const auto& [type, tag, func] : check_for_existance) {
            auto res = tokenCollection.findTag(type, tag);
            if (res == tokenCollection.end()) {
                func();
            }
        }
    }

    namespace 
    {
        bool interna_analyseAndFix(TokenCollection& tokenCollection, TokenCollection::tc_it& start, TokenCollection::tc_it& end)
        {
#if DEBUG_FIXER
            std::cout << "//////////////////////////////////////////////////////////////////////////////////////\n";
            std::cout << "Start token: ";
            output_token(*start);
            std::cout << "End token: ";
            output_token(*end);
            std::cout << "//////////////////////////////////////////////////////////////////////////////////////\n";
#endif

            std::vector<Token> open_tokens{};
            

            for (auto it = start; it != end; it++) {
                auto& token = *it;
#if DEBUG_FIXER
                std::cout << "Current tag: ";
                output_token(token);
#endif

                if (token.isOpenType()) {
                    open_tokens.push_back(token);
                }

                else if (token.isCloseType()) {
                    if (open_tokens.empty()) {
                        tokenCollection.erase(it);
                        return true; // call back
                    }

                    if (!open_tokens.back().isPartner(token)) {
                        if (tokenCollection.allClosed()) {
                            token.tag = open_tokens.back().tag;
                        }
                        else {
                            //auto find_closest_opened_tag = tokenCollection.findTag(it, TokenType::TAG_OPEN);
                            tokenCollection.insert(it, open_tokens.back().generateOposite());
                            return true; // call back
                        }
                    }

                    open_tokens.pop_back();
                }
            }

            return false;
        }
    }


    void Fixer::analyseAndFix(TokenCollection& tokenCollection)
    {
        bool run{ true };

        while (run) {
            auto body_open = tokenCollection.findTag("body");
            auto body_close = tokenCollection.findTag(TokenType::TAG_CLOSE, "body", TokenCollection::FindType::END);

            run = interna_analyseAndFix(tokenCollection, ++body_open, body_close);
        }
#if 0
        OutputCppTokens o{};
        std::filesystem::path p{ std::string{"token_dump.txt"} };
        o.output(tokenCollection.getAllTokens(), "tokens", &p);

        //output_tokens(tokenCollection.getAllTokens());
        exit(-1);
#endif
    }

}
