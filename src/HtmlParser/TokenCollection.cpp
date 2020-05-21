#include "TokenCollection.h"
#include <algorithm> //std::find_if
#include <iterator> //std::distance

//#define DEBUG_TOKENCOLLECTION
#ifdef DEBUG_TOKENCOLLECTION
#include <iostream>
#include "../tests/TokenizerTestHelper.h"
#endif
namespace HtmlParser
{

    TokenCollection* TokenCollection::add(TokenType type, const std::string& name, const std::string& attr)
    {
        std::size_t level{};        

        if (!m_tokens.empty()) {
            const auto& last_token = m_tokens.back();
            level = last_token.level;

            if (isOpenType(last_token)) {
                level += Token::level_amount;
            }

            if (isCloseType(type)) {
                level -= Token::level_amount;
            }
        }
        
        m_tokens.emplace_back(type, name, attr, level);

        return this;
    }

    void TokenCollection::set(const std::vector<Token>& tokens)
    {
        m_tokens = tokens;
        calculateLevelValues();
    }

    void TokenCollection::insert(const cit position, const Token& token)
    {
        m_tokens.insert(position, token);
        calculateLevelValues();
    }

    void TokenCollection::insert(const cit position, const std::vector<Token>& tokens)
    {
        m_tokens.insert(position, tokens.begin(), tokens.end());
        calculateLevelValues();
    }

    TokenCollection::cit TokenCollection::findTag(cit startingPoint, const TokenSearch& tokenSearch, const FindType mode) const
    {
        auto func = [&tokenSearch](const Token& token)
        {
            bool ret{ true };

#ifdef DEBUG_TOKENCOLLECTION
            std::cout << "Current tag inspecting: ";
            output_token(token);        
#endif

            if (tokenSearch.level_given && tokenSearch.level != 0)
                ret = tokenSearch.level == token.level;
            if (tokenSearch.code_given)
                ret = tokenSearch.code == token.code && ret;
            if (tokenSearch.tag_given && ret)
                ret = tokenSearch.tag == token.tag;
            if (tokenSearch.type_given && ret)
                ret = tokenSearch.type == token.type;

            return ret;
        };

        if (mode == FindType::BEGINNING)
            return std::find_if<cit>(startingPoint,m_tokens.cend(), func);

        auto spoint = m_tokens.rbegin();

        if (startingPoint != m_tokens.begin())
            spoint += std::distance <cit>(startingPoint, m_tokens.cend());

        auto res = std::find_if(spoint, m_tokens.rend(), func);

        if (res == m_tokens.rend()) return m_tokens.end();
        return res.base() - 1;
    }

    void TokenCollection::calculateLevelValues()
    {
        std::size_t level{};

        for (auto& token : m_tokens) {
            token.level = level;

            if (isOpenType(token)) {
                level += Token::level_amount;
            }
            else if (isCloseType(token)) {
                level -= Token::level_amount;
                token.level = level;
            }
        }
    }
}
