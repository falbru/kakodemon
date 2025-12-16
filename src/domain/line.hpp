#ifndef DOMAIN_LINE_HPP_INCLUDED
#define DOMAIN_LINE_HPP_INCLUDED

#include "domain/atom.hpp"

namespace domain
{

class Line
{
  public:
    Line();
    Line(std::vector<Atom> m_atoms);

    const Atom &at(int index) const;
    Line slice(int start_index, int end_index);

    const std::vector<Atom> &getAtoms() const;
    unsigned int size() const;

  private:
    std::vector<Atom> m_atoms;
};

class GlyphLine
{
  public:
    GlyphLine(std::vector<GlyphAtom> atoms);
    GlyphLine(const Line &line, Font *font);

    Line toLine() const;

    const std::vector<GlyphAtom> &getGlyphAtoms() const;

    float width() const;
    float height() const;

  private:
    std::vector<GlyphAtom> m_atoms;
};

} // namespace domain

#endif
