#ifndef RENDERLINE_HPP_INCLUDED
#define RENDERLINE_HPP_INCLUDED

#include "domain/facespan.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/line.hpp"
#include "domain/ports/font.hpp"

namespace domain
{

class RenderLine
{
  public:
    RenderLine(std::vector<GlyphMetrics> glyphs, std::vector<FaceSpan> face_spans);
    RenderLine(const Line &line, GlyphResolver &glyph_resolver);
    ~RenderLine();

    const std::vector<GlyphMetrics> &getGlyphs() const;
    const std::vector<FaceSpan> &getFaceSpans() const;
    size_t size() const;

    float width() const;
    float height() const;

    void truncate(float max_width, GlyphResolver &glyph_resolver);

    RenderLine split(size_t start, size_t end) const;

    std::vector<FaceSpan>::const_iterator faceSpanIteratorFromIndex(int index) const;

  private:
    std::vector<GlyphMetrics> m_glyphs;
    std::vector<FaceSpan> m_face_spans;
};

} // namespace domain

#endif
