#include "Tokenizer.h"
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <iostream>

namespace HtmlParser
{
    
    void Tokenizer::tokenize(const std::string& html)
    {
        m_html = html;
        tokenize();
    }
    void Tokenizer::tokenize()
    {
        m_position = 0;
        m_lines = 0;
        char c{};
        std::string text{};

        while ((c = getChar()) != 0) {
            switch (c) {
            case '<':
            {
                //ignore if has a space
                if (peek() == ' ')
                    continue;
                //TODO: Ignore comments: <!--code-->
                if (m_html.substr(m_position+1, 3) == "!--") {
                }

                if (isValidText(text)) {
                    m_tokens.emplace_back(TokenType::TEXT, text);
                    //std::cout << "Text size: " << text.size() << '\n';
                    //std::cout << "Text: " << text << '\n';                        
                }
                text.clear();


                //TODO: This will not work properly for comments
                //#1 - grab everything up to the first space or > and then figure out what type of tag it is
                //#2 - #1 will not work for comments because as soon as you do <!-- it starts a comments even if it is like <!--<p>

                //TODO: This will not work properly for reading files (but it might not be super complicated to fix)
                std::string::size_type res;
                if ((res = m_html.find('>', m_position)) != std::string::npos) {
                    auto len = res - m_position;
                    //do not take in < and >
                    auto ele = m_html.substr(m_position + 1, len - 1);
                    auto [elementName, attributes] = splitBySpace(ele);
                    auto type = getTagType(elementName);

                    if (type == TokenType::TAG_SELF_CLOSE && !attributes.empty() && attributes.back() == '/') attributes.pop_back();
                    if (type == TokenType::TAG_CLOSE) elementName.erase(0, 1); //remove /
                    if (type == TokenType::DOCTYPE) { //no need to store !doctype
                        elementName = attributes;
                        attributes.clear();
                    }

                    m_tokens.emplace_back(type, elementName, attributes);
                    m_position += len;
                }
                else {
                    //position is wrong!!
                    throw std::runtime_error{ "Element is not closed on line " + m_lines + std::string{" position " + m_position } };
                }
            }
            break;
            case '\n':
                m_lines++;
                [[fallthrough]];
            default:
                text += c;
            }

            m_position++;
        }
    }



    void Tokenizer::tokenize2()
    {
        m_position = 0;
        m_lines = 0;
        char c{};
        std::string text{};
        bool textNotValidTag{ false };
        m_tokens.clear();

        while ((c = getChar()) != 0) {
        
            if (c == '\n') 
                m_lines++;

            switch (m_state) {
                case TokenizerState::TEXT:
                {
                    if (c == '<' && peek() != ' ') {
                        if (isValidText(text)) {
                            m_tokens.emplace_back(TokenType::TEXT, text);
                        }

                        if (peek() == '!' && getString(4) == "<!--") {
                            m_state = TokenizerState::COMMENT;
                            m_position += 3;
                        }
                        else {
                            m_state = TokenizerState::TAG_OPENED;
                        }

                        text.clear();
                    } else {
                        text += c;
                    }
                }
                break;
                case TokenizerState::TAG_OPENED:
                    switch (c) {
                        case '>':
                            {
                                if (!textNotValidTag) {
                                    auto [elementName, attributes] = splitBySpace(text);
                                    auto type = getTagType(elementName);

                                    if (type == TokenType::TAG_SELF_CLOSE && !attributes.empty() && attributes.back() == '/') attributes.pop_back();
                                    if (type == TokenType::TAG_CLOSE) elementName.erase(0, 1); //remove /
                                    if (type == TokenType::DOCTYPE) { //no need to store !doctype
                                        elementName = attributes;
                                        attributes.clear();
                                    }

                                    m_tokens.emplace_back(type, elementName, attributes);
                                    m_state = TokenizerState::TEXT;
                                    text.clear();
                                } else text += c;
                            }
                        break;
                        case '"':
                            if (backPeek() != '\\') textNotValidTag = !textNotValidTag;
                            [[fallthrough]];
                        default:
                            text += c;
                    }
                break;
                case TokenizerState::COMMENT:
                {
                    if (c == '-' && getString(3) == "-->") {
                        m_state = TokenizerState::TEXT;
                        m_position += 2;
                    }
                }
                break;
                default:;
            }
            
            m_position++;
        }

    }


    char Tokenizer::getChar(int i)
    {
        if (m_position + i < m_html.size())
            return m_html[m_position + i];
        return '\0';
    }

    std::string Tokenizer::getString(std::size_t size)
    {
        std::string res{};
        std::size_t i{};

        while (i < size) {
            res += getChar();
            m_position++;
            i++;
        }

        m_position -= size;
        return res;
    }
   

    std::pair<std::string, std::string> Tokenizer::splitBySpace(const std::string& str)
    {
        auto toLowerTag = [](std::string& tag)
        {
            std::transform(tag.begin(), tag.end(), tag.begin(),
                [](unsigned char c) { return std::tolower(c); });
        };

        std::size_t sp;
        if ((sp = str.find(' ')) != std::string::npos) {
            auto tag = str.substr(0, sp);
            toLowerTag(tag);
            return {
                tag,
                str.substr(sp + 1)
            };
        }

        auto m = str;
        toLowerTag(m);

        return { m, "" };
    }

    TokenType Tokenizer::getTagType(const std::string& tag) const
    {
        if (tag == "!doctype") return TokenType::DOCTYPE;
        if (isSelfClosingTag(tag)) return TokenType::TAG_SELF_CLOSE;
        if (tag[0] == '/') return TokenType::TAG_CLOSE;
        return TokenType::TAG_OPEN;
    }

    bool Tokenizer::isSelfClosingTag(const std::string& tag) const
    {
        auto searchFor = (tag.back() == '/') ? tag.substr(0, tag.size() - 1) : tag;
        auto fi = std::find(m_self_closing_tags.begin(), m_self_closing_tags.end(), searchFor);
        return (fi != m_self_closing_tags.end());
    }

    bool Tokenizer::isValidText(const std::string& str) const
    {
        if (str.empty())
            return false;

        if (str.size() == 1 && (str[0] == ' ' || str[0] == '\n' || str[0] == '\t'))
            return false;

        auto cpy{ str };
        static std::string badChars{ " \n\r\t" };

        for (const auto& c : badChars) {
            cpy.erase(std::remove(cpy.begin(), cpy.end(), c), cpy.end());
        }

        return !cpy.empty();
    }
}
