#pragma once

#include <string>
#include <vector>
#include <utility> //std::pair
#include <array>

#include "Token.h"

namespace HtmlParser
{
    class Tokenizer
    {
        enum class TokenizerState
        {
            TEXT,
            TAG_OPENED,
            COMMENT
        };

        std::string m_html{};
        std::size_t m_position{};
        std::size_t m_lines{};
        std::vector<Token> m_tokens{};
        std::array<std::string, 17> m_self_closing_tags
        {
        "area","base","br","col","command",
        "embed","hr","img","input","keygen",
        "link","menuitem","meta","param","source",
        "track","wbr"
        };
        TokenizerState m_state{ TokenizerState::TEXT };

    public:
        Tokenizer() = default;
        Tokenizer(const std::string& html) : m_html(html)
        {
        }

        void tokenize(const std::string& html);

        void tokenize();
        void tokenize2();

        [[nodiscard]] std::size_t getLines() const
        {
            return m_lines;
        }

        [[nodiscard]] const std::vector<Token>& getTokens() const
        {
            return m_tokens;
        }

    private:
        char getChar(int i = 0);
        std::string getString(std::size_t size);

        char peek()
        {
            return getChar(1);
        }

        char backPeek()
        {
            if (m_position != 0)
                return getChar(-1);
            return '\0';
        }

        std::pair<std::string, std::string> splitBySpace(const std::string& str);

        bool isSelfClosingTag(const std::string& tag) const;

        bool isValidText(const std::string& str) const;

        TokenType getTagType(const std::string& tag) const;
    };
}
