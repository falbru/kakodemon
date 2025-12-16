#ifndef DOMAIN_LINES_HPP_INCLUDED
#define DOMAIN_LINES_HPP_INCLUDED

#include "domain/line.hpp"

namespace domain
{

class Lines
{
  public:
    Lines();
    Lines(std::vector<Line> lines);

    const Line &at(int index) const;

    const std::vector<Line> &getLines() const;
    unsigned int size() const;

  private:
    std::vector<Line> m_lines;
};

enum class WrapMode
{
    CHARACTER,
    WORD
};

class GlyphLines
{
  public:
    GlyphLines(const Lines &lines, Font *font);

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
