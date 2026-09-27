#include "renderline.hpp"
#include "domain/face.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/span.hpp"
#include <algorithm>
#include <stdexcept>

namespace domain
{

RenderLine::RenderLine(std::vector<GlyphMetrics> glyphs, std::vector<Span<Face>> face_spans,
                       std::vector<Span<Font *>> font_spans)
    : m_glyphs(std::move(glyphs)), m_face_spans(std::move(face_spans)), m_font_spans(std::move(font_spans))
{
    if (!(m_glyphs.empty() == m_face_spans.empty()))
    {
        throw std::invalid_argument("face spans can't be empty if glyphs exist");
    }

    if (!(m_glyphs.empty() == m_font_spans.empty()))
    {
        throw std::invalid_argument("font spans can't be empty if glyphs exist");
    }

    if (!std::is_sorted(m_face_spans.begin(), m_face_spans.end()))
    {
        std::sort(m_face_spans.begin(), m_face_spans.end());
    }

    if (!std::is_sorted(m_font_spans.begin(), m_font_spans.end()))
    {
        std::sort(m_font_spans.begin(), m_font_spans.end());
    }

    if (m_face_spans.size() >= 1 && m_face_spans.at(m_face_spans.size() - 1).start_index >= m_glyphs.size())
    {
        throw std::invalid_argument("start_index in face_spans exceeds glyphs size");
    }

    if (m_font_spans.size() >= 1 && m_font_spans.at(m_font_spans.size() - 1).start_index >= m_glyphs.size())
    {
        throw std::invalid_argument("start_index in font_spans exceeds glyphs size");
    }
}

RenderLine::RenderLine(const Line &line, GlyphResolver &glyph_resolver)
{
    m_glyphs.reserve(line.length());
    m_face_spans.reserve(line.size());

    int index = 0;

    Font *prev_font = nullptr;
    for (const auto &atom : line.getAtoms())
    {
        m_face_spans.push_back(Span<Face>(atom.getFace(), index));

        for (const auto &codepoint : atom.getContents())
        {
            auto glyph_with_font = glyph_resolver.resolveGlyphWithFont(codepoint);

            m_glyphs.push_back(glyph_with_font.glyph);

            if (prev_font == nullptr || glyph_with_font.font != prev_font)
            {
                m_font_spans.push_back(Span<Font *>(glyph_with_font.font, index));
                prev_font = glyph_with_font.font;
            }

            index++;
        }
    }
}

RenderLine::~RenderLine()
{
}

const std::vector<GlyphMetrics> &RenderLine::getGlyphs() const
{
    return m_glyphs;
}

const std::vector<Span<Face>> &RenderLine::getFaceSpans() const
{
    return m_face_spans;
}

const std::vector<Span<Font *>> &RenderLine::getFontSpans() const
{
    return m_font_spans;
}

size_t RenderLine::size() const
{
    return m_glyphs.size();
}

float RenderLine::width() const
{
    float width = 0.0f;
    for (const auto &glyph : m_glyphs)
    {
        width += glyph.advance;
    }
    return width;
}

float RenderLine::height() const
{
    float height = 0.0f;
    for (const auto &glyph : m_glyphs)
    {
        height = std::max((float)glyph.size.y, height);
    }
    return height;
}

void RenderLine::truncate(float max_width, GlyphResolver &glyph_resolver)
{
    float current_width = width();

    if (current_width < max_width)
        return;

    const auto &ellipsis_glyph = glyph_resolver.resolveGlyph(0x2026);
    float ellipsis_width = ellipsis_glyph.advance;

    if (max_width < ellipsis_width)
    {
        m_glyphs.clear();
        m_face_spans.clear();
        m_font_spans.clear();
        return;
    }

    int glyph_index = m_glyphs.size() - 1;
    while (glyph_index >= 0)
    {
        const auto &glyph = m_glyphs.at(glyph_index);
        current_width -= glyph.advance;
        glyph_index--;

        if (current_width + ellipsis_width <= max_width)
        {
            break;
        }
    }

    if (glyph_index < 0)
    {
        m_glyphs.clear();
        m_face_spans.erase(m_face_spans.begin() + 1, m_face_spans.end());
        m_font_spans.erase(m_font_spans.begin() + 1, m_font_spans.end());
    }
    else
    {
        auto face_span_it = spanIteratorFromIndex(m_face_spans, glyph_index);
        if (face_span_it != m_face_spans.end())
        {
            m_face_spans.erase(face_span_it + 1, m_face_spans.end());
        }

        auto font_span_it = spanIteratorFromIndex(m_font_spans, glyph_index);
        if (font_span_it != m_font_spans.end())
        {
            m_font_spans.erase(font_span_it + 1, m_font_spans.end());
        }

        m_glyphs.erase(m_glyphs.begin() + glyph_index + 1, m_glyphs.end());
    }

    m_glyphs.push_back(ellipsis_glyph);
}

RenderLine RenderLine::split(size_t start, size_t end) const
{
    if (start >= m_glyphs.size())
    {
        return RenderLine({}, {}, {});
    }
    if (end > m_glyphs.size())
    {
        end = m_glyphs.size();
    }
    if (start >= end)
    {
        return RenderLine({}, {}, {});
    }

    std::vector<GlyphMetrics> new_glyphs(m_glyphs.begin() + start, m_glyphs.begin() + end);

    auto face_start_it = spanIteratorFromIndex(m_face_spans, start);
    auto face_end_it = spanIteratorFromIndex(m_face_spans, end - 1);
    std::vector<Span<Face>> new_face_spans(face_start_it, face_end_it + 1);
    for (int i = 0; i < new_face_spans.size(); i++)
    {
        if (new_face_spans[i].start_index < start)
        {
            new_face_spans[i].start_index = 0;
        }
        else
        {
            new_face_spans[i].start_index -= start;
        }
    }

    auto font_start_it = spanIteratorFromIndex(m_font_spans, start);
    auto font_end_it = spanIteratorFromIndex(m_font_spans, end - 1);
    std::vector<Span<Font *>> new_font_spans(font_start_it, font_end_it + 1);
    for (int i = 0; i < new_font_spans.size(); i++)
    {
        if (new_font_spans[i].start_index < start)
        {
            new_font_spans[i].start_index = 0;
        }
        else
        {
            new_font_spans[i].start_index -= start;
        }
    }

    return RenderLine(new_glyphs, new_face_spans, new_font_spans);
}

} // namespace domain
