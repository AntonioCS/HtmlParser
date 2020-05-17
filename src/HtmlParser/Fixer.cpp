#include "Fixer.h"

#include <stdexcept>
#include <utility>

namespace HtmlParser
{

    void Fixer::process(std::vector<Token>& tokens)
    {
        if (!tokens.empty()) {
            m_tokens = tokens;

            ensureProperStructure();
            addWeightValues();
            analyseAndFix();

            //swapOpenCloseTokens(tokens);

            /*
            std::size_t n{ tokens.size() -1};
            std::size_t i{ 0 }; 
            while (i < n) { 
                auto type = tokens.at(i).type;

                switch (type) {
                    case TokenType::DOCTYPE:
                    case TokenType::TAG_SELF_CLOSE:
                    case TokenType::TEXT:
                    case TokenType::CODE:
                        i++;
                    break;

                    case TokenType::TAG_OPEN:


                    break;
                    case TokenType::TAG_CLOSE:
                        i++;
                    break;
                    default:
                        throw std::runtime_error{ "Fixer: Unknown tag type" };
                }
            }*/

            tokens = m_tokens;
        }
    }

    void Fixer::ensureProperStructure()
    {
        if (m_tokens.front().type != TokenType::DOCTYPE) {
            m_tokens.insert(m_tokens.begin(), { TokenType::DOCTYPE,"html" });
        }

        if (!isStartHtmlTag(m_tokens.at(1))) {
            m_tokens.insert(m_tokens.begin() + 1, { HtmlParser::TokenType::TAG_OPEN, "html" });
        }

        if (!isEndHtmlTag(m_tokens.back())) {
            m_tokens.emplace_back(HtmlParser::TokenType::TAG_CLOSE, "html");
        }

        //check for head
        if (!isTagMatch(m_tokens.at(2), "head")) {
            std::vector<Token> basic_head{
                {TokenType::TAG_OPEN, "head"},
                    //{TokenType::TAG_OPEN, "title"},
                    //{TokenType::TAG_CLOSE, "title"},
                {TokenType::TAG_CLOSE, "head"}
            };

            m_tokens.insert(m_tokens.begin() + 2, basic_head.begin(), basic_head.end());
        }

        auto body_pos = findTag("body");
        if (body_pos == m_tokens.end()) {
            auto head_close_pos = findTag("head", TokenType::TAG_CLOSE);

            m_tokens.insert(head_close_pos + 1, { HtmlParser::TokenType::TAG_OPEN, "body" });
            m_tokens.insert(m_tokens.end() - 1, { HtmlParser::TokenType::TAG_CLOSE, "body" });            
        }
    }

    void Fixer::addWeightValues()
    {
        std::size_t weight{0};
        for (auto& token : m_tokens) {
            if (isOpenType(token)) {
                weight += 10;
                token.weight = weight;
            }
            else if (isCloseType(token)) {
                token.weight = weight;
                weight -= 10;
            }
        }
    }

    int Fixer::findOpenTag(std::size_t index, std::size_t weight)
    {
        while (index--) {
            auto& token = m_tokens[index];
            if (isOpenType(token) && token.weight == weight) {
                return index;
            }
        }

        return -1;
    }
    
    void Fixer::analyseAndFix()
    {
        for (std::size_t i{}; i < m_tokens.size(); i++) {
            auto& token = m_tokens[i];

            if (isCloseType(token)) {
                auto index = findOpenTag(i, token.weight);
                if (index == -1) {//no open tag
                    m_tokens.insert(m_tokens.begin() + i, 
                        { HtmlParser::TokenType::TAG_OPEN, token.tag , "", token.weight });
                    i++;
                } else {
                    if (m_tokens[index].tag != token.tag) {
                        m_tokens[index].tag = token.tag;
                    }
                }
            }
        }
    }


    void Fixer::swapOpenCloseTokens()
    {
        for (std::size_t i{}; i < m_tokens.size(); i++) {
            if (isOpenType(m_tokens[i]) &&
                i + 1 < m_tokens.size() &&
                isCloseType(m_tokens[i + 1]) &&
                m_tokens[i].tag != m_tokens[i + 1].tag) 
            {

                std::swap(m_tokens[i], m_tokens[i + 1]);
                i++;
            }
        }
    }

    std::pair<std::size_t, std::size_t> Fixer::findBodyRange()
    {
        std::size_t start{}, end{};
        for (std::size_t i{}; i < m_tokens.size(); i++) {
            if (isOpenType(m_tokens.at(i)) && isTagMatch(m_tokens.at(i), "body")) {
                start = i + 1;
                break;
            }
        }

        auto bend = m_tokens.end() - 2;
        if (isCloseType(*bend) && isTagMatch(*bend, "body")) {
            end = std::distance(m_tokens.begin(), bend);
        } else {
            //crap
        }

        return { start, end };
    }

    bool Fixer::isOpenType(const Token& token)
    {
        return token.type == TokenType::TAG_OPEN;
    }

    bool Fixer::isCloseType(const Token& token)
    {
        return token.type == TokenType::TAG_CLOSE;
    }

    bool Fixer::isTagMatch(const Token& token, const std::string& match)
    {
        return token.tag == match;
    }

    bool Fixer::isStartHtmlTag(const Token& token)
    {
        return isOpenType(token) && isTagMatch(token, "html");
    }

    bool Fixer::isEndHtmlTag(const Token& token)
    {
        return isCloseType(token) && isTagMatch(token, "html");
    }

    std::vector<Token>::const_iterator Fixer::findTag(const std::string& tag, TokenType type)
    {
        return std::find_if(
            m_tokens.begin(), m_tokens.end(), [&tag, &type](const Token& token)
            {
                return (token.tag == tag && token.type == type);
            });
    }
}
