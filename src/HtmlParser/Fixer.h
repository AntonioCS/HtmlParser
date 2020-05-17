#pragma once
#include <vector>
#include <HtmlParser/Token.h>

namespace HtmlParser
{


    class Fixer
    {
        std::vector<Token> m_tokens;
    public:
        Fixer() = default;
        ~Fixer() = default;

        void process(std::vector<Token>&);
    private:
        void ensureProperStructure();

        void addWeightValues();


        int findOpenTag(std::size_t index, std::size_t weight);

        void analyseAndFix();



        void swapOpenCloseTokens();

        std::pair<std::size_t, std::size_t> findBodyRange();

        bool isOpenType(const Token& token);

        bool isCloseType(const Token& token);

        bool isTagMatch(const Token& token, const std::string& match);

        bool isStartHtmlTag(const Token& token);

        bool isEndHtmlTag(const Token& token);

        std::vector<Token>::const_iterator findTag(const std::string& tag, TokenType type = TokenType::TAG_OPEN);
    };
}
