#ifndef DOMAIN_GLYPHLINESBUILDER_HPP_INCLUDED
#define DOMAIN_GLYPHLINESBUILDER_HPP_INCLUDED

#include "domain/fontmanager.hpp"
#include "domain/glyphlines.hpp"
#include "domain/lines.hpp"
#include "domain/ports/font.hpp"

namespace domain
{

class GlyphLinesBuilder
{
  public:
    static GlyphLines build(const Lines &lines, Font *font, FontManager *font_manager);
    static GlyphLine build(const Line &line, Font *font, FontManager *font_manager);
    static GlyphAtom build(const Atom &atom, Font *font, FontManager *font_manager);

    static GlyphLines build(const Lines &lines, Font *font);
    static GlyphLine build(const Line &line, Font *font);
    static GlyphAtom build(const Atom &atom, Font *font);

  private:
    static std::vector<GlyphRun> buildGlyphRuns(const UTF8String &contents, Font *font, FontManager *font_manager);
    static std::vector<GlyphRun> buildGlyphRuns(const UTF8String &contents, Font *font);
};

} // namespace domain

#endif
