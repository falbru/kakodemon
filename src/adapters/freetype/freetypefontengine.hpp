#ifndef FREETYPEENGINE_HPP_INCLUDED
#define FREETYPEENGINE_HPP_INCLUDED

#include "domain/ports/fontengine.hpp"
#include "domain/ports/fontresolver.hpp"
#include "freetypelibrary.hpp"
#include <freetype/freetype.h>
#include <ft2build.h>
#include <functional>
#include <map>
#include <memory>
#include FT_FREETYPE_H

using FontEngineFactory = std::function<std::unique_ptr<domain::FontEngine>(const domain::FontMatch &)>;

class FreeTypeFontEngine : public domain::FontEngine
{
  public:
    FreeTypeFontEngine(std::shared_ptr<FreeTypeLibrary> library, const std::string &path, int size, int face_index);
    ~FreeTypeFontEngine();

    static FontEngineFactory createFactory(std::shared_ptr<FreeTypeLibrary> library);

    bool hasGlyph(domain::Codepoint c) const override;
    std::optional<domain::RasterizedGlyph> rasterizeGlyph(domain::Codepoint c) override;
    std::optional<domain::RasterizedGlyph> rasterizeFallbackGlyph() override;

    float getAscender() const override;
    float getDescender() const override;
    float getLineHeight() const override;
    int getSize() const override;

  private:
    std::shared_ptr<FreeTypeLibrary> m_library;
    FT_Face m_face;

    float m_ascender;
    float m_descender;
    float m_line_height;
    int m_requested_size;
    float m_scale;

    std::map<domain::Codepoint, domain::RasterizedGlyph> m_glyphs;
    std::optional<domain::RasterizedGlyph> fallback_glyph;
};

#endif
