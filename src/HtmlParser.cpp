#include <HtmlParser/HtmlParser.h>
#include <HtmlParser/Fixer.h>

namespace HtmlParser
{
    std::unique_ptr<Document> HtmlParser::parse(const std::string& html) const
    {
        Tokenizer tokenizer{ html };

        return parse(tokenizer);
    }

    std::unique_ptr<Document> HtmlParser::parse(const std::filesystem::path& file) const
    {
        Tokenizer tokenizer{ file };
        
        return parse(tokenizer);
    }

    std::unique_ptr<Document> HtmlParser::parse()
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

    std::unique_ptr<Document> HtmlParser::result()
    {
        const Parser parser{ m_tokenizer.getTokens() };
        return parser.parse();
    }

    std::unique_ptr<Document> HtmlParser::parse(Tokenizer& tokenizer) const
    {
        tokenizer.tokenize();

        auto res = tokenizer.getCollection();
        Fixer fixer;
        fixer.process(res);

        const Parser parser{ res.getAllTokens() };
        return parser.parse();
    }
}
