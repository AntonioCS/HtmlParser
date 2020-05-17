#pragma once

#include <HtmlParser/Tokenizer.h>
#include <HtmlParser/Parser.h>

namespace HtmlParser
{

    class HtmlParser
    {
        enum class DataType
        {
            FULL,
            CHUNKS
        };

        Tokenizer m_tokenizer;
        std::string m_html;
        std::filesystem::path m_file;
    public:
        HtmlParser() = default;
        HtmlParser(const std::string& html) : m_html(html) {}
        HtmlParser(const std::filesystem::path& file) : m_file(file) {}

        Document parse(const std::string& html);
        Document parse(const std::filesystem::path& file);
        Document parse();

        void parseChunks(const std::string& html);
        void parseChunks(const std::filesystem::path& file);

        Document result();
    private:

        Document parse(Tokenizer& tokenizer) const;
    };
}