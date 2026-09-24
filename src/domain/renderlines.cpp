#include "domain/renderlines.hpp"
#include "domain/codepointstring.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/renderline.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace domain
{

RenderLines::RenderLines(std::vector<RenderLine> lines, float line_height) : m_lines(lines), m_line_height(line_height)
{
}

RenderLines::RenderLines(const Lines &lines, GlyphResolver &glyph_resolver, float line_height)
    : m_line_height(line_height)
{
    m_lines.reserve(lines.size());

    for (const auto &line : lines.getLines())
    {
        m_lines.push_back(RenderLine(line, glyph_resolver));
    }
}

const std::vector<RenderLine> &RenderLines::getLines() const
{
    return m_lines;
}

float RenderLines::getLineHeight() const
{
    return m_line_height;
}

size_t RenderLines::size() const
{
    return m_lines.size();
}

float RenderLines::width() const
{
    float width = 0.0f;
    for (const auto &line : m_lines)
    {
        width = std::max(width, line.width());
    }
    return width;
}

float RenderLines::height() const
{
    return m_line_height * size();
}

void RenderLines::wrap(float max_width, RenderLinesWrapMode mode)
{
    if (width() <= max_width)
    {
        return;
    }

    std::vector<RenderLine> wrapped_lines;
    wrapped_lines.reserve(m_lines.size() * 2);

    for (const auto &line : m_lines)
    {
        RenderLine remaining = line;

        if (remaining.size() == 0)
        {
            wrapped_lines.push_back(std::move(remaining));
            continue;
        }

        if (remaining.getGlyphs()[0].advance > max_width)
        {
            throw std::runtime_error("max_width is too small to wrap contents");
        }

        while (remaining.size() > 0)
        {
            float current_width = 0.0f;
            size_t char_index = 0;
            size_t last_word_boundary = 0;
            bool exceeded = false;

            for (; char_index < remaining.size(); ++char_index)
            {
                const auto &glyph = remaining.getGlyphs()[char_index];

                if (mode == RenderLinesWrapMode::WORD && char_index > 0 && isWhitespace(glyph.codepoint))
                {
                    last_word_boundary = char_index;
                }

                current_width += glyph.advance;

                if (current_width > max_width)
                {
                    exceeded = true;
                    break;
                }
            }

            if (!exceeded)
            {
                wrapped_lines.push_back(remaining);
                break;
            }

            size_t wrapped_stop = char_index;
            size_t remainder_start = wrapped_stop;
            if (mode == RenderLinesWrapMode::WORD && last_word_boundary > 0)
            {
                wrapped_stop = last_word_boundary;
                remainder_start = last_word_boundary + 1;
            }

            RenderLine wrapped = remaining.split(0, wrapped_stop);
            wrapped_lines.push_back(wrapped);

            remaining = remaining.split(remainder_start, remaining.size());
        }
    }

    m_lines = std::move(wrapped_lines);
}

} // namespace domain
