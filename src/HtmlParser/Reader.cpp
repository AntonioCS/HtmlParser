#include "Reader.h"

namespace  HtmlParser
{
    char Reader::getChar()
    {
        m_caused_read = false;
        return m_has_file ? getCharFromFile() : getCharFromStr();
    }

    std::string Reader::getString(const std::size_t size)
    {
        if (m_has_file) {
            if (size > (m_buffer_size * 2)) {
                return {};
            }

            if (m_line_position + size < m_buffer_size) {
                return std::string{ m_buffer.data.begin() + m_line_position, m_buffer.data.begin() + m_line_position + size };
            }

            std::string result{ m_buffer.data.begin() + m_line_position, m_buffer.data.end() };
            auto diff = m_buffer.data.end() - (m_buffer.data.begin() + m_line_position);

            savePositions();
            if (read()) {
                result += std::string{ m_buffer.data.begin(), m_buffer.data.begin() + (size - diff) };
                loadSavedPositions();
                swapToBackBuffer();
            }

            return result;
        }

        auto res = std::string{ m_str_buffer.substr(m_line_position, size) };
        return res;
    }

    char Reader::peek()
    {
        savePositions();
        incPosition();

        auto c = getChar();

        loadSavedPositions();

        if (m_caused_read) {
            swapToBackBuffer();
        }

        return c;
    }

    char Reader::backPeek()
    {
        if (m_global_position > 0) {
            if (m_has_file && m_line_position == 0 && !m_back_buffer.data.empty()) {
                return m_back_buffer.data.back();
            }

            savePositions();
            decPosition();
            auto c = getChar();
            loadSavedPositions();

            return c;
        }

        return '\0';
    }

    char Reader::getCharFromFile()
    {
        if (m_buffer.data.empty()) //first time
            read();
        else if (m_line_position >= m_buffer_size) {
            if (!read())
                return '\0';
        }

        return m_buffer.data[m_line_position];
    }

    char Reader::getCharFromStr()
    {
        return (!m_str_buffer.empty() && m_line_position < m_str_buffer.size())
            ? m_str_buffer[m_line_position] : '\0';
    }

    void Reader::swapToBackBuffer()
    {
        std::swap(m_buffer, m_back_buffer);
        m_global_position = m_buffer.line_start + m_line_position;
        m_file.seekg(m_global_position);
    }

    bool Reader::read()
    {
        if (!m_file.eof()) {
            m_line_position = 0;

            if (m_back_buffer.line_start > m_buffer.line_start) {
                swapToBackBuffer();
            }
            else {
                m_caused_read = true;
                m_back_buffer = m_buffer;

                m_buffer.data.assign(m_buffer_size, 0);
                m_file.read(m_buffer.data.data(), m_buffer_size);
                m_buffer.line_start = static_cast<std::size_t>(m_file.tellg());
                m_global_position = (m_buffer.line_start < 0 ? 0 : m_buffer.line_start);
            }

            return true;
        }

        return false;
    }

    bool Reader::setReadDataFromFile(const std::filesystem::path& file)
    {
        if (std::filesystem::exists(file) && std::filesystem::is_regular_file(file)) {
            resetPositions();
            m_file.open(file.c_str());
            return (m_has_file = m_file.is_open());
        }

        return false;
    }

    void Reader::setReadDataFromString(const std::string_view data)
    {
        resetPositions();
        m_str_buffer = data;
    }

    void Reader::savePositions()
    {
        m_saved_positions.first = m_line_position;
        m_saved_positions.second = m_global_position;
    }

    void Reader::loadSavedPositions()
    {
        m_line_position = m_saved_positions.first;
        m_global_position = m_saved_positions.second;
    }

    void Reader::resetPositions()
    {
        m_global_position = 0;
        m_line_position = 0;
    }

    void Reader::incPosition(const std::size_t increase)
    {
        auto buffer = m_has_file ? m_buffer_size : m_str_buffer.size();
        m_line_position += increase;

        if (m_global_position + increase < buffer)
            m_global_position += increase;
        else
            m_global_position = buffer - 1;
    }

    void Reader::decPosition(const std::size_t decrease)
    {
        if (static_cast<int>(m_line_position) - decrease >= 0) m_line_position -= decrease;
        if (static_cast<int>(m_global_position) - decrease >= 0) m_global_position -= decrease;
    }
}
