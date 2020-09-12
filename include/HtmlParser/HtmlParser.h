#pragma once

#include "Tokenizer.h"
#include "Parser.h"

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

        std::unique_ptr<Document> parse(const std::string& html) const;
        std::unique_ptr<Document> parse(const std::filesystem::path& file) const;
        std::unique_ptr<Document> parse();

        void parseChunks(const std::string& html);
        void parseChunks(const std::filesystem::path& file);

        std::unique_ptr<Document> result();
    private:

        std::unique_ptr<Document> parse(Tokenizer& tokenizer) const;
    };
}