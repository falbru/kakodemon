#ifndef RENDERLINES_HPP_INCLUDED
#define RENDERLINES_HPP_INCLUDED

#include "domain/lines.hpp"
#include "domain/renderline.hpp"
namespace domain
{

enum class RenderLinesWrapMode
{
    CHARACTER,
    WORD
};

class RenderLines
{
  public:
    RenderLines(std::vector<RenderLine> lines, float line_height);
    RenderLines(const Lines &lines, GlyphResolver &glyph_resolver, float line_height);

    const std::vector<RenderLine> &getLines() const;
    float getLineHeight() const;
    size_t size() const;

    float width() const;
    float height() const;

    void wrap(float max_width, RenderLinesWrapMode mode = RenderLinesWrapMode::WORD);

  private:
    std::vector<RenderLine> m_lines;
    float m_line_height;
};

} // namespace domain

#endif
