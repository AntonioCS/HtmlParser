#pragma once
#include <vector>
#include <HtmlParser/Token.h>

namespace HtmlParser
{
    class TokenCollection;


    class Fixer
    {
    public:
        Fixer() = default;
        ~Fixer() = default;

        void process(TokenCollection&);
    private:
        void ensureProperStructure(TokenCollection&);
        void analyseAndFix(TokenCollection&);
    };
}
