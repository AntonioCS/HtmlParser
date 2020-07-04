#pragma once

#include <vector>
#include "Token.h"

namespace HtmlParser
{
    class TokenCollection
    {
    public:
        enum class FindType
        {
            BEGINNING,
            END
        };
        using tc_it = std::vector<Token>::iterator;
        using tc_cit = std::vector<Token>::const_iterator;
    private:
        std::vector<std::size_t> m_token_codes;
        std::vector<Token> m_tokens;

        class TokenSearch
        {
        public:
            enum class NumValue
            {
                LEVEL,
                CODE
            };

            TokenType type{};
            std::string tag{};            
            std::size_t level{};
            std::size_t code{};
        
            bool type_given{ false };
            bool tag_given{ false };
            bool level_given{ false };
            bool code_given{ false };

            TokenSearch(const TokenType type) : type(type), type_given(true) {}
            TokenSearch(std::size_t num, NumValue nv = NumValue::LEVEL) {
                assignCorrectValue(num, nv);
            }
            TokenSearch(const std::string& tag) : tag(tag), tag_given(true) {}
            TokenSearch(const std::string& tag, std::size_t num, NumValue nv = NumValue::LEVEL) : tag(tag), tag_given(true)
            {
                assignCorrectValue(num, nv);
            }

            TokenSearch(const std::string& tag, std::size_t level, std::size_t code) : tag(tag), level(level), code(code), tag_given(true), level_given(true), code_given(true) {}
            TokenSearch(const TokenType type, const std::string& tag) : type(type), tag(tag), type_given(true), tag_given(true) {}
            TokenSearch(const TokenType type, const std::string& tag, std::size_t level) : type(type), tag(tag), level(level), type_given(true), tag_given(!tag.empty()), level_given(true){}
            TokenSearch(const TokenType type, const std::string& tag, const std::size_t level, std::size_t code)
               : type(type),
                tag(tag),               
                level(level),
                code(code),
                type_given(true),
                tag_given(true),
                level_given(true),
                code_given(true)
            { }
        private:
            void assignCorrectValue(std::size_t num, NumValue nv)
            {
                if (nv == NumValue::LEVEL) {
                    level = num;
                    level_given = true;
                }
                else {
                    code = num;
                    code_given = true;
                }
            }

        };

        std::size_t m_total_open{};
        std::size_t m_total_close{};
    public:
        TokenCollection() = default;
        TokenCollection(const std::vector<Token>& tokens)
        {
            set(tokens);
        }

        TokenCollection(tc_it start, tc_it end)
        {
            for (auto i = start+1; i != end; i++) {
                add(*i);
            }

            calculateValues();
        }

        TokenCollection* add(TokenType, const std::string& name, const std::string& attr = "");
        TokenCollection* add(Token&);
        void set(const std::vector<Token>&);

        tc_it begin() { return m_tokens.begin(); }
        [[nodiscard]] tc_cit begin() const { return m_tokens.cbegin(); }
        tc_it end() { return m_tokens.end(); }
        [[nodiscard]] tc_cit end() const { return m_tokens.cend(); }

        std::vector<Token>& getAllTokens() { return m_tokens; }

        void clear() { m_tokens.clear(); }
        [[nodiscard]] bool empty() const { return m_tokens.empty(); }

        [[nodiscard]] tc_it findTag(const std::string& tag, FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ tag }, mode); }
        [[nodiscard]] tc_it findTag(const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ tag, level }, mode); }
        [[nodiscard]] tc_it findTag(const std::string& tag, std::size_t level, std::size_t code, const FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ tag, level , code }, mode); }
        [[nodiscard]] tc_it findTag(TokenType type, FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ type }, mode); }
        [[nodiscard]] tc_it findTag(TokenType type, const std::string& tag, FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ type, tag }, mode); }
        [[nodiscard]] tc_it findTag(TokenType type, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) { return findTag(m_tokens.begin(), TokenSearch{ type, tag, level }, mode); }

        [[nodiscard]] tc_it findTag(tc_it startingPoint, const std::string& tag, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ tag }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ tag, level }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, const std::string& tag, std::size_t level, std::size_t code, const FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ tag, level, code }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, TokenType type, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ type }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, TokenType type, const std::string& tag, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ type, tag }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, TokenType type, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ type, tag, level }, mode); }
        [[nodiscard]] tc_it findTag(tc_it startingPoint, TokenType type, std::size_t level, FindType mode = FindType::BEGINNING) { return findTag(startingPoint, TokenSearch{ type,"", level }, mode); }

        void insert(tc_cit position, Token&&);
        void insert(std::size_t position, Token&&);
        void insert(tc_cit position, std::vector<Token>&&);
        void insert(std::size_t position, std::vector<Token>&&);

        void erase(tc_cit position);
        void erase(tc_cit start, tc_cit end);

        Token& front() { return m_tokens.front(); }
        [[nodiscard]] const Token& front() const { return m_tokens.front(); }
        Token& back() { return m_tokens.back(); }
        [[nodiscard]] const Token& back() const { return m_tokens.back(); }

        [[nodiscard]] std::size_t getTotalOpen() const { return m_total_open;  }
        [[nodiscard]] std::size_t getTotalClose() const { return m_total_close; }
        [[nodiscard]] bool allClosed() const { return m_total_close == m_total_open; }

        [[nodiscard]] std::vector<std::size_t> getTokenCodes() const { return m_token_codes; }
        [[nodiscard]] std::vector<std::size_t> getTokenCodes(tc_it start, tc_it end) const;

        [[nodiscard]] std::pair<std::size_t, std::size_t> countOpenCloseTagsBewtween(tc_cit start, tc_cit end) const;

    private:
        [[nodiscard]] tc_it findTag(tc_it startingPoint, const TokenSearch&, FindType mode = FindType::BEGINNING);

        bool isOpenType(const Token& token) { return token.type == TokenType::TAG_OPEN; }
        bool isOpenType(TokenType type) { return type == TokenType::TAG_OPEN; }
        bool isCloseType(const Token& token) { return token.type == TokenType::TAG_CLOSE; }
        bool isCloseType(TokenType type) { return type == TokenType::TAG_CLOSE; }

        void calculateValues();
    };
}
