#include "HtmlParser.h"
#include <HtmlParser/Tokenizer.h>
#include <HtmlParser/Parser.h>

#include "Fixer.h"

namespace HtmlParser
{
    Document HtmlParser::parse(const std::string& html)
    {
        Tokenizer tokenizer{ html };

        return parse(tokenizer);
    }

    Document HtmlParser::parse(const std::filesystem::path& file)
    {
        Tokenizer tokenizer{ file };
        
        return parse(tokenizer);
    }

    Document HtmlParser::parse()
    {
        if (!m_html.empty() || !m_file.empty() || m_tokenizer.hasTokens()) {

            if (!m_html.empty())
                return parse(m_html);

            if (!m_file.empty())
                return parse(m_file);

            return result();
        }

        throw std::runtime_error{ "Nothing has been set" };
    }

    void HtmlParser::parseChunks(const std::string& html)
    {
        m_tokenizer.tokenize(html);
    }

    void HtmlParser::parseChunks(const std::filesystem::path& file)
    {
        m_tokenizer.tokenize(file);
    }

    Document HtmlParser::result()
    {
        Parser parser{ m_tokenizer.getTokens() };
        return parser.parse();
    }

    Document HtmlParser::parse(Tokenizer& tokenizer) const
    {
        tokenizer.tokenize();

        auto res = tokenizer.getCollection();
        Fixer fixer;
        fixer.process(res);

        Parser parser{ res.getAllTokens() };
        return parser.parse();
    }
}
