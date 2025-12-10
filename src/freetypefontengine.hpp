#ifndef FREETYPEENGINE_HPP_INCLUDED
#define FREETYPEENGINE_HPP_INCLUDED

#include "fontengine.hpp"
#include <freetype/freetype.h>
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

class FreeTypeFontEngine : public FontEngine
{
  public:
    FreeTypeFontEngine(const std::string path, int size);
    ~FreeTypeFontEngine();

    bool hasGlyph(Codepoint c) const override;
    std::optional<RasterizedGlyph> rasterizeGlyph(Codepoint c) override;

    float getAscender() const override;
    float getLineHeight() const override;

  private:
    FT_Library m_library;
    FT_Face m_face;

    float m_ascender;
    float m_line_height;

    std::map<Codepoint, RasterizedGlyph> m_glyphs;
};

#endif
