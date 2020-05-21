#pragma once

#include <vector>
#include <HtmlParser/Token.h>

namespace HtmlParser
{
    class TokenCollection
    {
        std::vector<Token> m_tokens;
        using it = std::vector<Token>::iterator;
        using cit = std::vector<Token>::const_iterator;

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
            TokenSearch(const TokenType type, const std::string& tag, std::size_t level) : type(type), tag(tag), level(level), type_given(true), tag_given(true), level_given(true){}
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

    public:
        enum class FindType
        {
            BEGINNING,
            END
        };

    public:
        TokenCollection() = default;
        TokenCollection(const std::vector<Token>& tokens)
        {
            set(tokens);
        }

        void clear()
        {
            m_tokens.clear();
        }

        TokenCollection* add(TokenType, const std::string& name, const std::string& attr = "");
        void set(const std::vector<Token>&);

        it begin() { return m_tokens.begin(); }
        [[nodiscard]] cit begin() const { return m_tokens.cbegin(); }
        it end() { return m_tokens.end(); }
        [[nodiscard]] cit end() const { return m_tokens.cend(); }

        std::vector<Token>& getAllTokens() { return m_tokens; }

        [[nodiscard]] bool empty() const { return m_tokens.empty(); }

        [[nodiscard]] cit findTag(const std::string& tag, FindType mode = FindType::BEGINNING) const { return findTag(m_tokens.begin(), TokenSearch{ tag }, mode); }
        [[nodiscard]] cit findTag(const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) const { return findTag(m_tokens.begin(), TokenSearch{ tag, level }, mode); }
        [[nodiscard]] cit findTag(const std::string& tag, std::size_t level, std::size_t code, const FindType mode = FindType::BEGINNING) const { return findTag(m_tokens.begin(), TokenSearch{ tag, level , code}, mode); }
        [[nodiscard]] cit findTag(TokenType type, FindType mode = FindType::BEGINNING) const { return findTag(m_tokens.begin(), TokenSearch{ type }, mode); }
        [[nodiscard]] cit findTag(TokenType type, const std::string& tag, FindType mode = FindType::BEGINNING) const { return findTag(m_tokens.begin(), TokenSearch{ type, tag }, mode); }
        [[nodiscard]] cit findTag(TokenType type, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) const
        {
            return findTag(m_tokens.begin(), TokenSearch{ type, tag, level }, mode);
        }

        [[nodiscard]] cit findTag(cit startingPoint, const std::string& tag, FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ tag }, mode); }
        [[nodiscard]] cit findTag(cit startingPoint, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ tag, level }, mode); }
        [[nodiscard]] cit findTag(cit startingPoint, const std::string& tag, std::size_t level, std::size_t code, const FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ tag, level }, mode); }
        [[nodiscard]] cit findTag(cit startingPoint, TokenType type, FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ type }, mode); }
        [[nodiscard]] cit findTag(cit startingPoint, TokenType type, const std::string& tag, FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ type, tag }, mode); }
        [[nodiscard]] cit findTag(cit startingPoint, TokenType type, const std::string& tag, std::size_t level, FindType mode = FindType::BEGINNING) const { return findTag(startingPoint, TokenSearch{ type, tag, level }, mode); }

        void insert(cit position, const Token&);
        void insert(cit position, const std::vector<Token>&);

        Token& front() { return m_tokens.front(); }
        [[nodiscard]] const Token& front() const { return m_tokens.front(); }
        Token& back() { return m_tokens.back(); }
        [[nodiscard]] const Token& back() const { return m_tokens.back(); }

    private:
        [[nodiscard]] cit findTag(cit startingPoint, const TokenSearch&, FindType mode = FindType::BEGINNING) const;

        bool isOpenType(const Token& token) { return token.type == TokenType::TAG_OPEN; }
        bool isOpenType(TokenType type) { return type == TokenType::TAG_OPEN; }
        bool isCloseType(const Token& token) { return token.type == TokenType::TAG_CLOSE; }
        bool isCloseType(TokenType type) { return type == TokenType::TAG_CLOSE; }

        void calculateLevelValues();
    };
}