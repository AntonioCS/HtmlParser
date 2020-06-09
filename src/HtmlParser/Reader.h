#pragma once

#include <fstream>
#include <filesystem>

namespace HtmlParser {
    
    constexpr std::size_t HTML_PARSER_READER_BUFFER_SIZE{ 16384 }; //16kb
    
    class Reader
    {
        std::size_t m_buffer_size{ HTML_PARSER_READER_BUFFER_SIZE };
        std::string_view m_str_buffer;
        std::size_t m_global_position{};
        std::size_t m_line_position{};
        bool m_has_file{ false };
        std::ifstream m_file;
        bool m_caused_read{ false };
        std::pair <std::size_t, std::size_t> m_saved_positions;

        struct ReadData
        {
            int line_start{};
            std::vector<char> data{};
        };

        ReadData m_buffer{};
        ReadData m_back_buffer{};
    public:
        Reader() = default;
        Reader(const std::string_view data) : m_str_buffer(data) { }
        Reader(const std::filesystem::path& file) { setReadDataFromFile(file); }
        Reader(std::size_t buffer_size, const std::string_view data) : Reader(data) { m_buffer_size = buffer_size; }
        Reader(std::size_t buffer_size, const std::filesystem::path& file) : Reader(file) { m_buffer_size = buffer_size; }

        [[nodiscard]] std::size_t getBufferSize() const { return m_buffer_size; }

        void incPosition(const std::size_t increase = 1);

        void decPosition(const std::size_t decrease = 1);

        char getChar();

        std::string getString(const std::size_t size);

        //std::string getLine();

        char peek();

        char backPeek();

        bool setReadDataFromFile(const std::filesystem::path& file);

        void setReadDataFromString(const std::string_view data);

        void resetPositions();
    private:
        char getCharFromFile();

        char getCharFromStr();

        void swapToBackBuffer();

        bool read();

        void savePositions();

        void loadSavedPositions();
    };
}