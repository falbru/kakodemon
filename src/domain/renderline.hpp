#ifndef RENDERLINE_HPP_INCLUDED
#define RENDERLINE_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/glyphresolver.hpp"

#include "domain/faceresolver.hpp"
#include "domain/line.hpp"
#include "domain/ports/font.hpp"
#include "domain/span.hpp"

namespace domain
{

class RenderLine
{
  public:
    RenderLine(std::vector<GlyphMetrics> glyphs, std::vector<Span<ResolvedFace>> face_spans,
               std::vector<Span<Font *>> font_spans);
    RenderLine(const Line &line, GlyphResolver &glyph_resolver, const FaceResolver &face_resolver);
    ~RenderLine();

    const std::vector<GlyphMetrics> &getGlyphs() const;
    const std::vector<Span<ResolvedFace>> &getFaceSpans() const;
    const std::vector<Span<Font *>> &getFontSpans() const;
    size_t size() const;

    float width() const;
    float height() const;

    void truncate(float max_width, GlyphResolver &glyph_resolver);

    RenderLine split(size_t start, size_t end) const;

  private:
    std::vector<GlyphMetrics> m_glyphs;
    std::vector<Span<ResolvedFace>> m_face_spans;
    std::vector<Span<Font *>> m_font_spans;
};

} // namespace domain

#endif
