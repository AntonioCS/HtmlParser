#include "Fixer.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <HtmlParser/TokenCollection.h>
#include "../tests/TestHelper.h"

namespace HtmlParser
{

    void Fixer::process(std::vector<Token>& tokens)
    {
        if (!tokens.empty()) {
            ensureProperStructure(tokens);            
            analyseAndFix(tokens);
        }
    }

    void Fixer::process(TokenCollection& tokenCollection)
    {
        if (!tokenCollection.empty()) {
            ensureProperStructure(tokenCollection);
        }
    }

    void Fixer::ensureProperStructure(std::vector<Token>& tokens)
    {
        if (tokens.front().type != TokenType::DOCTYPE) {
            tokens.insert(tokens.begin(), { TokenType::DOCTYPE,"html" });
        }

        if (!isStartHtmlTag(tokens.at(1))) {
            tokens.insert(tokens.begin() + 1, { HtmlParser::TokenType::TAG_OPEN, "html" });
        }

        if (!isEndHtmlTag(tokens.back())) {
            tokens.emplace_back(HtmlParser::TokenType::TAG_CLOSE, "html");
        }

        //check for head
        if (!isTagMatch(tokens.at(2), "head")) {
            std::vector<Token> basic_head{
                {TokenType::TAG_OPEN, "head"},
                    //{TokenType::TAG_OPEN, "title"},
                    //{TokenType::TAG_CLOSE, "title"},
                {TokenType::TAG_CLOSE, "head"}
            };

            tokens.insert(tokens.begin() + 2, basic_head.begin(), basic_head.end());
        }

        auto body_pos = findTag(tokens, "body");
        if (body_pos == tokens.end()) {
            auto head_close_pos = findTag(tokens,"head", -1, TokenType::TAG_CLOSE);

            tokens.insert(head_close_pos + 1, { HtmlParser::TokenType::TAG_OPEN, "body" });
            tokens.insert(tokens.end() - 1, { HtmlParser::TokenType::TAG_CLOSE, "body" });            
        }
    }

    void Fixer::ensureProperStructure(TokenCollection&)
    {
    }

    void Fixer::addWeightValues(std::vector<Token>& tokens)
    {
        std::size_t weight{0};

        for (auto& token : tokens) {
            if (isOpenType(token)) {
                token.level = weight;
                //weight += m_weight_amount + token.tag.size();
                weight += m_weight_amount;
            }
            else if (isCloseType(token)) {               
                //weight -= (m_weight_amount + token.tag.size());
                weight -= m_weight_amount;
                token.level = weight;
            }
            else {
                token.level = weight;
            }
        }
    }

    void Fixer::analyseAndFix(std::vector<Token>& tokens, std::size_t i)
    {
        //@todo: There might be a way to only readd weitghts to the incorrect tags and not redo everything
        addWeightValues(tokens);
        
        std::filesystem::path tokens_file{ std::string{"output_tokens.txt" } };

        OutputCppTokens o{
            true, false, false, true , {}, {}
            //{TokenType::TEXT, TokenType::CODE, TokenType::TAG_SELF_CLOSE},
          //  {"script", "li", "ul", "a", "table", "td", "tr", "i", "b", "span", "strong"}
        };
       //output_cpp_tokens(tokens, "big_file", &tokens_file);
        o.output(tokens, "big_file", &tokens_file);

        //exit(-1);
        //*/

        //std::cout << "I = " << i << '\n';
        //output_tokens(tokens);
        ////std::cout << "------------------------------------------------------\n";        
        const auto [body_start_it, body_end_it] = findBodyRange(tokens);
        bool reCall{ false };

        for (auto it = body_start_it + i; it != body_end_it; it++) {
            i = std::distance(body_start_it, it);
            auto& token = *it;
            std::cout << "Current tag: ";
            output_token(token);
            
            if (isCloseType(token)) {
                auto res = findTag(tokens, "", it, token.level, TokenType::TAG_OPEN, FindType::END);
                if (res == tokens.end() || res->tag == "body") {                    
                    tokens.erase(it);
                    reCall = true;
                    break;
                }

                if (res->tag != token.tag) {                    
                    tokens.insert(res, { TokenType::TAG_OPEN, token.tag });
                    reCall = true;
                    break;
                }
            }            
            else if (isOpenType(token)) {
                auto res = findTag(tokens, token.tag, it, token.level, TokenType::TAG_CLOSE);

                if (res == tokens.end()) {
                    if (token.level == m_weight_of_body_first_element) {
                        tokens.erase(it);
                    }
                    else {
                        res = findTag(tokens, "", it, token.level, TokenType::TAG_CLOSE);
                        if (res->tag == "html")
                            res = body_end_it;
                        tokens.insert(res, { TokenType::TAG_CLOSE, token.tag });
                    }
                    reCall = true;
                    break;
                }
            }
        }

        if (reCall) {
            analyseAndFix(tokens, i);
        }
    }


    //std::pair<std::size_t, std::size_t> Fixer::findBodyRange(const std::vector<Token>& tokens)
    std::pair < Fixer::token_it, Fixer::token_it> Fixer::findBodyRange(const std::vector<Token>& tokens)
    {
        auto startIt = findTag(tokens, "body");
        auto endIt = findTag(tokens, "body", -1, TokenType::TAG_CLOSE, FindType::END);
        
        if (startIt == tokens.end() || endIt == tokens.end())
            throw std::runtime_error{ "Unable to find body" };

        return { startIt,  endIt };
    }

    //std::vector<Token>::const_iterator Fixer::findTag(const std::vector<Token>& tokens, const std::string& tag, TokenType type)
    Fixer::token_it Fixer::findTag(
        const std::vector<Token>& tokens,
        const std::string& tag,
        token_it startingPoint,
        int weight,
        TokenType type,
        FindType mode)
    {
        auto func = [&tag, &weight, &type](const Token& token)
        {
            bool ret{ true };
            
            if (weight > 0)
                ret = static_cast<std::size_t>(weight) == token.level;
            if (!tag.empty())
                ret = tag == token.tag && ret;

            return (type == token.type && ret);
        };

        if (mode == FindType::BEGINNING)
            return std::find_if(startingPoint, tokens.end(), func);

        auto spoint = tokens.rbegin();

        if (startingPoint != tokens.begin())
            spoint += std::distance(startingPoint, tokens.end());

        auto res = std::find_if(spoint, tokens.rend(), func);

        if (res == tokens.rend()) return tokens.end();
        return res.base() - 1;
    }

}
