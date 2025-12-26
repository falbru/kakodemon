#ifndef DOMAIN_GLYPHATOM_HPP_INCLUDED
#define DOMAIN_GLYPHATOM_HPP_INCLUDED

#include "domain/atom.hpp"
#include "domain/ports/font.hpp"

namespace domain
{

struct GlyphRun
{
    std::vector<GlyphMetrics> glyphs;
    Font *font;
};

class GlyphAtom
{
  public:
    GlyphAtom(const std::vector<GlyphRun> &runs, const Face &face);

    Atom toAtom() const;

    const std::vector<GlyphRun> &getRuns() const;
    const Face &getFace() const;

    float width() const;

  private:
    std::vector<GlyphRun> m_runs;
    Face m_face;
};

} // namespace domain

#endif
