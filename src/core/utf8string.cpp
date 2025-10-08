#include "core/utf8string.hpp"

UTF8String::UTF8String()
{
}

// Use kakoune's utf8 encoding and decoding for reference:
// https://github.com/mawww/kakoune/blob/e3263887653ce5ec817c1aad51389c9600dfeabe/src/utf8.hh

UTF8String::UTF8String(std::string string)
{
    auto it = string.cbegin();
    auto end = string.cend();
    while (it != end)
    {
        const unsigned char byte = static_cast<unsigned char>(*it);
        Codepoint codepoint = 0;

        if (byte <= 0x7F)
        {
            codepoint = byte;
            ++it;
        }
        else if ((byte & 0xE0) == 0xC0)
        {
            if (it + 1 >= end)
                break;
            codepoint = ((byte & 0x1F) << 6) | (static_cast<unsigned char>(*(it + 1)) & 0x3F);
            it += 2;
        }
        else if ((byte & 0xF0) == 0xE0)
        {
            if (it + 2 >= end)
                break;
            codepoint = ((byte & 0x0F) << 12) | ((static_cast<unsigned char>(*(it + 1)) & 0x3F) << 6) |
                        (static_cast<unsigned char>(*(it + 2)) & 0x3F);
            it += 3;
        }
        else if ((byte & 0xF8) == 0xF0)
        {
            if (it + 3 >= end)
                break;
            codepoint = ((byte & 0x07) << 18) | ((static_cast<unsigned char>(*(it + 1)) & 0x3F) << 12) |
                        ((static_cast<unsigned char>(*(it + 2)) & 0x3F) << 6) |
                        (static_cast<unsigned char>(*(it + 3)) & 0x3F);
            it += 4;
        }
        else
        {
            ++it;
            continue;
        }
        m_codepoints.push_back(codepoint);
    }
}

std::string UTF8String::toString() const
{
    std::string result;

    for (Codepoint codepoint : m_codepoints)
    {
        if (codepoint <= 0x7F)
        {
            result += static_cast<char>(codepoint);
        }
        else if (codepoint <= 0x7FF)
        {
            result += static_cast<char>(0xC0 | (codepoint >> 6));
            result += static_cast<char>(0x80 | (codepoint & 0x3F));
        }
        else if (codepoint <= 0xFFFF)
        {
            result += static_cast<char>(0xE0 | (codepoint >> 12));
            result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (codepoint & 0x3F));
        }
        else if (codepoint <= 0x10FFFF)
        {
            result += static_cast<char>(0xF0 | (codepoint >> 18));
            result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
            result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (codepoint & 0x3F));
        }
    }

    return result;
}

int UTF8String::size() const
{
    return m_codepoints.size();
}

Codepoint UTF8String::at(unsigned int index) const
{
    return m_codepoints.at(index);
}

void UTF8String::addCodepoint(Codepoint codepoint)
{
    m_codepoints.push_back(codepoint);
}

bool isWhitespace(Codepoint cp)
{
    return cp == 0x09 || cp == 0x0A || cp == 0x0B || cp == 0x0C || cp == 0x0D || cp == 0x20 || cp == 0x85 ||
           cp == 0xA0 || cp == 0x1680 || (cp >= 0x2000 && cp <= 0x200A) || cp == 0x2028 || cp == 0x2029 ||
           cp == 0x202F || cp == 0x205F || cp == 0x3000;
}

void UTF8String::trim(TrimDirection direction)
{
    if (m_codepoints.empty())
        return;

    size_t start = 0;
    size_t end = m_codepoints.size();

    if (direction == TrimDirection::Left || direction == TrimDirection::Both)
    {
        while (start < end && isWhitespace(m_codepoints[start]))
        {
            ++start;
        }
    }

    if (direction == TrimDirection::Right || direction == TrimDirection::Both)
    {
        while (end > start && isWhitespace(m_codepoints[end - 1]))
        {
            --end;
        }
    }

    if (start > 0 || end < m_codepoints.size())
    {
        m_codepoints = std::vector<Codepoint>(m_codepoints.begin() + start, m_codepoints.begin() + end);
    }
}

UTF8String UTF8String::substring(size_t start, size_t length) const
{
    UTF8String result;

    if (start >= m_codepoints.size())
    {
        return result;
    }

    size_t end = std::min(start + length, m_codepoints.size());
    result.m_codepoints = std::vector<Codepoint>(m_codepoints.begin() + start, m_codepoints.begin() + end);

    return result;
}

UTF8String UTF8String::substring(size_t start) const
{
    return substring(start, m_codepoints.size() - start);
}
