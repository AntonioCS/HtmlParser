#include <HtmlParser/Tokenizer.h>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace HtmlParser
{
    void Tokenizer::tokenize(const std::filesystem::path& file, TokenizerMode mode)
    {
        m_reader.setReadDataFromFile(file);
        tokenize(mode);
    }

    void Tokenizer::tokenize(const std::string& html, TokenizerMode mode)
    {
        m_reader.setReadDataFromString(std::string_view{ html });
        tokenize(mode);
    }
   
    void Tokenizer::tokenize(TokenizerMode mode)
    {
        if (mode == TokenizerMode::NORMAL) {
            m_lines = 0;
            m_tokens.clear();
            m_content.clear();
            m_state = TokenizerState::TEXT;
        }

        m_reader.resetPositions();
        char c;

        while ((c = m_reader.getChar()) != 0) {
        
            if (c == '\n') 
                m_lines++;

            switch (m_state) {
                case TokenizerState::CODE:
                {
                    m_after_quotes_jump_to_state = m_state;
                    tokenizerStateHelper(c, '<', [this]()
                        {                                   // check if it's the same tag
                            if (m_reader.peek() == '/' && m_reader.getString(m_code_block.size()) == m_code_block) {
                                if (!m_content.empty())
                                    m_tokens.add(TokenType::CODE, m_content);

                                m_state = TokenizerState::TAG_OPENED;
                                m_content.clear();
                            }
                        });
                }
                break;
                case TokenizerState::TEXT:
                {
                    if (c == '<' && m_reader.peek() != ' ') {
                        if (!m_content.empty() && isValidText(m_content)) {
                            m_tokens.add(TokenType::TEXT, m_content);
                        }

                        if (m_reader.peek() == '!' && m_reader.getString(4) == "<!--") {
                            m_state = TokenizerState::COMMENT;
                            m_reader.incPosition(3);
                        }
                        else {
                            m_state = TokenizerState::TAG_OPENED;
                        }

                        m_content.clear();
                    } else
                        m_content += c;
                }
                break;
                case TokenizerState::TAG_OPENED:
                    m_after_quotes_jump_to_state = m_state;
                    tokenizerStateHelper(c, '>', [this]()
                        {
                            auto [elementName, attributes] = splitBySpace(m_content);
                            //to catch tags that I don't have in m_self_closing_tags as is the case for <path /> which is a svg element that self closes
                            auto type = (m_reader.backPeek() == '/') ? TokenType::TAG_SELF_CLOSE : getTagType(elementName);

                            if (type == TokenType::TAG_SELF_CLOSE && !attributes.empty() && attributes.back() == '/')
                                attributes.pop_back();
                            else if (type == TokenType::TAG_CLOSE || type == TokenType::DOCTYPE)
                                elementName.erase(0, 1); //remove / || remove ! from !doctype                            

                            m_tokens.add(type, elementName, attributes);

                            if (type != TokenType::TAG_CLOSE && (elementName == "script" || elementName == "style")) {
                                m_state = TokenizerState::CODE;
                                m_code_block = elementName;
                                m_code_block.insert(0, "</");
                                m_code_block.append(">");
                            }
                            else {
                                m_state = TokenizerState::TEXT;
                            }

                            m_content.clear();
                        });
                break;
                case  TokenizerState::IN_QUOTES:
                    m_content += c;

                    if (c == m_quote_end_state_symbol && m_reader.backPeek() != '\\') {
                        m_state = m_after_quotes_jump_to_state;
                    }
                break;
                case TokenizerState::COMMENT:
                {
                    if (c == '-' && m_reader.getString(3) == "-->") {
                        m_state = TokenizerState::TEXT;
                        m_reader.incPosition(2);
                    }
                }
                break;
                default:;
            }
            
            m_reader.incPosition();
        }
    }

    std::pair<std::string, std::string> Tokenizer::splitBySpace(const std::string& str) const
    {
        const auto toLowerTag = [](std::string& tag)
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
        const auto searchFor = (tag.back() == '/') ? tag.substr(0, tag.size() - 1) : tag;
        const auto fi = std::find(m_self_closing_tags.begin(), m_self_closing_tags.end(), searchFor);
        return (fi != m_self_closing_tags.end());
    }

    bool Tokenizer::isValidText(const std::string& str) const
    {
        if (str.size() == 1 && (str[0] == ' ' || str[0] == '\n' || str[0] == '\t'))
            return false;

        auto cpy{ str };
        static std::string badChars{ " \n\r\t" };

        for (const auto& c : badChars) {
            cpy.erase(std::remove(cpy.begin(), cpy.end(), c), cpy.end());
        }

        return !cpy.empty();
    }

    void Tokenizer::tokenizerStateHelper(char c, char x, const std::function<void()>& func)
    {
        if (c == x) {
            func();
        }
        else {
            if (c == '"' || c == '\'') {
                m_state = TokenizerState::IN_QUOTES;
                m_quote_end_state_symbol = c;
            }

            m_content += c;
        }
    }

}
