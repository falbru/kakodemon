#ifndef DOMAIN_GLYPHLINES_HPP_INCLUDED
#define DOMAIN_GLYPHLINES_HPP_INCLUDED

#include "domain/glyphline.hpp"
#include "domain/lines.hpp"

namespace domain
{

enum class WrapMode
{
    CHARACTER,
    WORD
};

class GlyphLines
{
  public:
    GlyphLines(const std::vector<GlyphLine> &lines, float line_height);

    Lines toLines() const;
    const std::vector<GlyphLine> &getGlyphLines() const;
    unsigned int size() const;

    float width() const;
    float height() const;

    void wrap(float max_width, WrapMode mode = WrapMode::WORD);

  private:
    std::vector<GlyphLine> m_lines;
    float m_line_height;
};

} // namespace domain

#endif
