#include "renderline.hpp"
#include "domain/facespan.hpp"
#include "domain/glyphresolver.hpp"
#include <algorithm>
#include <stdexcept>

namespace domain
{

RenderLine::RenderLine(std::vector<GlyphMetrics> glyphs, std::vector<FaceSpan> face_spans)
    : m_glyphs(std::move(glyphs)), m_face_spans(std::move(face_spans))
{
    if (!std::is_sorted(m_face_spans.begin(), m_face_spans.end()))
    {
        std::sort(m_face_spans.begin(), m_face_spans.end());
    }

    if (m_face_spans.size() >= 1 && m_face_spans.at(m_face_spans.size() - 1).start_index >= m_glyphs.size())
    {
        throw std::invalid_argument("start_index in FaceSpan exceeds glyphs size");
    }
}

RenderLine::RenderLine(const Line &line, GlyphResolver &glyph_resolver)
{
    m_glyphs.reserve(line.length());
    m_face_spans.reserve(line.size());

    int index = 0;
    for (const auto &atom : line.getAtoms())
    {
        m_face_spans.push_back(FaceSpan(atom.getFace(), index));

        for (const auto &codepoint : atom.getContents())
        {
            m_glyphs.push_back(glyph_resolver.resolveGlyph(codepoint));
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

const std::vector<FaceSpan> &RenderLine::getFaceSpans() const
{
    return m_face_spans;
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
    }
    else
    {
        auto face_span_it = faceSpanIteratorFromIndex(glyph_index);
        if (face_span_it != m_face_spans.end())
        {
            m_face_spans.erase(face_span_it + 1, m_face_spans.end());
        }
        m_glyphs.erase(m_glyphs.begin() + glyph_index + 1, m_glyphs.end());
    }

    m_glyphs.push_back(ellipsis_glyph);
}

RenderLine RenderLine::split(size_t start, size_t end) const
{
    if (start >= m_glyphs.size())
    {
        return RenderLine({}, {});
    }
    if (end > m_glyphs.size())
    {
        end = m_glyphs.size();
    }
    if (start >= end)
    {
        return RenderLine({}, {});
    }

    std::vector<GlyphMetrics> new_glyphs(m_glyphs.begin() + start, m_glyphs.begin() + end);

    auto start_it = faceSpanIteratorFromIndex(start);
    auto end_it = faceSpanIteratorFromIndex(end - 1);
    std::vector<FaceSpan> new_face_spans(start_it, end_it + 1);
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

    return RenderLine(new_glyphs, new_face_spans);
}

std::vector<FaceSpan>::const_iterator RenderLine::faceSpanIteratorFromIndex(int index) const
{
    if (m_face_spans.empty())
    {
        return m_face_spans.end();
    }

    if (index < m_face_spans.front().start_index)
    {
        return m_face_spans.end();
    }

    if (index >= m_face_spans.back().start_index)
    {
        return m_face_spans.end() - 1;
    }

    size_t left = 0;
    size_t right = m_face_spans.size() - 1;

    while (left < right)
    {
        size_t mid = (left + right) / 2;

        if (m_face_spans[mid].start_index <= index)
        {
            if (mid + 1 < m_face_spans.size() && m_face_spans[mid + 1].start_index <= index)
            {
                left = mid + 1;
            }
            else
            {
                return m_face_spans.begin() + mid;
            }
        }
        else
        {
            right = mid - 1;
        }
    }

    if (m_face_spans[left].start_index <= index)
    {
        return m_face_spans.begin() + left;
    }

    return m_face_spans.end();
}

} // namespace domain
