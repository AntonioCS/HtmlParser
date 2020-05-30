#include "TokenCollection.h"
#include <algorithm> //std::find_if
#include <iostream>
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
                m_total_close++;
            }
        }

        if (isOpenType(type)) m_total_open++;

        m_tokens.emplace_back(type, name, attr, level);

        return this;
    }

    TokenCollection* TokenCollection::add(Token& token)
    {
        m_tokens.push_back(token);
        return this;
    }

    void TokenCollection::set(const std::vector<Token>& tokens)
    {
        m_tokens = tokens;
        calculateValues();
    }

    void TokenCollection::insert(tc_cit position, Token&& token)
    {
        m_tokens.insert(position, token);
        calculateValues();
    }

    void TokenCollection::insert(std::size_t position, Token&& token)
    {
        insert(m_tokens.begin() + position, std::move(token));
    }

    void TokenCollection::insert(tc_cit position, std::vector<Token>&& tokens)
    {
        m_tokens.insert(position, tokens.begin(), tokens.end());
        calculateValues();
    }

    void TokenCollection::insert(std::size_t position, std::vector<Token>&& tokens)
    {
        insert(m_tokens.begin() + position, std::move(tokens));
    }

    void TokenCollection::erase(tc_cit position)
    {
        m_tokens.erase(position);
        calculateValues();
    }

    void TokenCollection::erase(tc_cit start, tc_cit end)
    {
        m_tokens.erase(start, end);
        calculateValues();
    }

    std::pair<std::size_t, std::size_t> TokenCollection::countOpenCloseTagsBewtween(tc_cit start, tc_cit end) const
    {
        std::size_t total_open{};
        std::size_t total_close{};

        for (auto it = start +1; it != end; it++) {
            if (it->isOpenType()) 
                total_open++;
            else if (it->isCloseType()) 
                total_close++;
        }

        return { total_open, total_close };
    }

    TokenCollection::tc_it TokenCollection::findTag(tc_it startingPoint, const TokenSearch& tokenSearch, const FindType mode)
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
            return std::find_if(startingPoint,m_tokens.end(), func);

        auto spoint = m_tokens.rbegin();

        if (startingPoint != m_tokens.begin())
            spoint += std::distance<tc_cit>(startingPoint, m_tokens.end());

        auto res = std::find_if(spoint, m_tokens.rend(), func);

        if (res == m_tokens.rend()) return m_tokens.end();
        return res.base() - 1;
    }

    void TokenCollection::calculateValues()
    {
        std::size_t level{};
        m_token_codes.clear();
        m_total_open = 0;
        m_total_close = 0;

       for (auto& token : m_tokens) {
            token.level = level;

            if (token.isOpenType()) {
                level += Token::level_amount;
                m_total_open++;
            }

            else if (token.isCloseType()) {
                if (token.level != 0) {
                    level -= Token::level_amount;
                    token.level = level;
                }
                m_total_close++;
            }
        }
    }

    inline std::vector<std::size_t> TokenCollection::getTokenCodes(tc_it start, tc_it end) const
    {
        std::vector<std::size_t> codes{};

        for (auto i = start; i != end; i++) {
            if (i->isOpenOrCloseType()) {
                codes.push_back(i->code);
            }
        }

        return codes;
    }
}
