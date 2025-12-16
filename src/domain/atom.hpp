#ifndef DOMAIN_ATOM_HPP_INCLUDED
#define DOMAIN_ATOM_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/ports/font.hpp"
#include "utf8string.hpp"

namespace domain
{

class Atom
{
  public:
    Atom(UTF8String contents, Face face);

    const UTF8String &getContents() const;
    const Face &getFace() const;
    unsigned int size() const;

    Atom slice(int start_index);
    Atom slice(int start_index, int length);

  private:
    UTF8String m_contents;
    Face m_face;
};

class GlyphAtom
{
  public:
    GlyphAtom(const std::vector<GlyphMetrics> &glyphs, const Face &face);
    GlyphAtom(const Atom &atom, Font *font);

    Atom toAtom() const;

    const std::vector<GlyphMetrics> &getGlyphs() const;
    const Face &getFace() const;

    float width() const;

  private:
    std::vector<GlyphMetrics> m_glyphs;
    Face m_face;
};

} // namespace domain

#endif
