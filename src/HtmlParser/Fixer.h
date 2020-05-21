#pragma once
#include <vector>
#include <HtmlParser/Token.h>

namespace HtmlParser
{
    class TokenCollection;


    class Fixer
    {
        enum class FindType
        {
            BEGINNING,
            END
        };
    
        const std::size_t m_weight_amount{ 10 };
        const std::size_t m_weight_of_body_first_element{20}; //@NOTE: This might not be correct if elements in HEAD tag are not correct
    public:
        Fixer() = default;
        ~Fixer() = default;

        void process(std::vector<Token>&);
        void process(TokenCollection&);
    private:
        using token_it = std::vector<Token>::const_iterator;

        void ensureProperStructure(std::vector<Token>&);
        void ensureProperStructure(TokenCollection&);

        void addWeightValues(std::vector<Token>&);

        void analyseAndFix(std::vector<Token>&, std::size_t i = 1);
        void analyseAndFix(TokenCollection&);

        std::pair<token_it, token_it> findBodyRange(const std::vector<Token>& tokens);

        bool isOpenType(const Token& token) { return token.type == TokenType::TAG_OPEN; }
        bool isCloseType(const Token& token) { return token.type == TokenType::TAG_CLOSE; }
        bool isTagMatch(const Token& token, const std::string& match) { return token.tag == match; }
        bool isStartHtmlTag(const Token& token) { return isOpenType(token) && isTagMatch(token, "html"); }
        bool isEndHtmlTag(const Token& token) { return isCloseType(token) && isTagMatch(token, "html"); }

        token_it findTag(
            const std::vector<Token>& tokens,
            const std::string& tag,
            int weight = -1,
            TokenType type = TokenType::TAG_OPEN,
            FindType mode = FindType::BEGINNING
        )
        {
            return findTag(tokens, tag, tokens.begin(), weight, type, mode);
        }

        token_it findTag(
            const std::vector<Token>& tokens,
            const std::string& tag,
            token_it,
            //std::size_t startingPoint = 0, 
            int weight = -1, 
            TokenType type = TokenType::TAG_OPEN, 
            FindType mode = FindType::BEGINNING
        );
        
        
    };
}
