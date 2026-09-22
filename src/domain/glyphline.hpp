#ifndef DOMAIN_GLYPHLINE_HPP_INCLUDED
#define DOMAIN_GLYPHLINE_HPP_INCLUDED

#include "domain/fontmanager.hpp"
#include "domain/glyphatom.hpp"
#include "domain/line.hpp"

namespace domain
{

class GlyphLine
{
  public:
    GlyphLine(std::vector<GlyphAtom> atoms);

    Line toLine() const;

    const std::vector<GlyphAtom> &getGlyphAtoms() const;

    float width() const;
    float height() const;

    void truncate(float max_width, Font *font, FontManager *font_manager);

  private:
    std::vector<GlyphAtom> m_atoms;
};

} // namespace domain

#endif
