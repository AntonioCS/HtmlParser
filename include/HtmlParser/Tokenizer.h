#pragma once

#include <string>
#include <vector>
#include <utility> //std::pair
#include <array>
#include <filesystem>
#include <functional>
#include "Token.h"
#include "TokenCollection.h"
#include "Reader.h"

namespace HtmlParser
{

    //TODO: Start using the Reader
    class   Tokenizer
    {
    public:
        enum class TokenizerMode
        {
            NORMAL,
            CHUNKS
        };

    private:
        enum class TokenizerState
        {
            TEXT,
            CODE, //for script and style content
            TAG_OPENED,
            COMMENT,
            IN_QUOTES //single or double
        };

        std::size_t m_lines{};
        //std::vector<Token> m_tokens{};
        TokenCollection m_tokens;
        std::array<std::string, 17> m_self_closing_tags
        {
        "area","base","br","col","command",
        "embed","hr","img","input","keygen",
        "link","menuitem","meta","param","source",
        "track","wbr"
        };
        TokenizerState m_state{ TokenizerState::TEXT };
        Reader m_reader{};
        std::string m_content{};
        char m_quote_end_state_symbol{};
        std::string m_code_block{};
        TokenizerState m_after_quotes_jump_to_state{};
    public:
        Tokenizer() = default;
        Tokenizer(const std::string& html)
        {
            m_reader.setReadDataFromString(html);
        }

        Tokenizer(const std::filesystem::path& file)
        {
            if (!m_reader.setReadDataFromFile(file)) {
                throw std::runtime_error{ "Unable to open file" };
            }
        }

        void tokenize(const std::filesystem::path& file, TokenizerMode mode = TokenizerMode::NORMAL);
        void tokenize(const std::string& html, TokenizerMode mode = TokenizerMode::NORMAL);
        void tokenize(TokenizerMode mode = TokenizerMode::NORMAL);

        [[nodiscard]] std::size_t getLines() const
        {
            return m_lines;
        }

        [[nodiscard]] const std::vector<Token>& getTokens()
        {
           return m_tokens.getAllTokens();
        }

        TokenCollection& getCollection()
        {
            return m_tokens;
        }

        [[nodiscard]] bool hasTokens() const
        {
            return !m_tokens.empty();
        }

    private:
        std::pair<std::string, std::string> splitBySpace(const std::string& str);
        [[nodiscard]] bool isSelfClosingTag(const std::string& tag) const;
        [[nodiscard]] bool isValidText(const std::string& str) const;
        [[nodiscard]] TokenType getTagType(const std::string& tag) const;

        void tokenizerStateHelper(char, char, const std::function<void()>&);
    };
}
