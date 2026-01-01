#ifndef APPLICATION_APPLICATIONBUILDER_HPP_INCLUDED
#define APPLICATION_APPLICATIONBUILDER_HPP_INCLUDED

#include "application/application.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/ports/fontresolver.hpp"
#include <functional>
#include <memory>

enum class PlatformType
{
    OPENGL_GLFW
};

enum class FontResolverType
{
    FONTCONFIG
};

enum class FontEngineType
{
    FREETYPE
};

using FontEngineFactory = std::function<std::unique_ptr<domain::FontEngine>(const domain::FontMatch &)>;

class ApplicationBuilder
{
  public:
    ApplicationBuilder();

    ApplicationBuilder &withPlatform(PlatformType platform);
    ApplicationBuilder &withFontResolver(FontResolverType resolver);
    ApplicationBuilder &withFontEngine(FontEngineType engine);

    std::unique_ptr<Application> build();

  private:
    PlatformType m_platform;
    FontResolverType m_font_resolver;
    FontEngineType m_font_engine;

    std::unique_ptr<domain::FontResolver> createFontResolver();
    FontEngineFactory createFontEngineFactory();
};

#endif
