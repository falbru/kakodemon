#include "applicationbuilder.hpp"
#include "adapters/fontconfig/fontconfigresolver.hpp"
#include "adapters/freetype/freetypefontengine.hpp"
#include "adapters/freetype/freetypelibrary.hpp"
#include "adapters/opengl/window.hpp"

ApplicationBuilder::ApplicationBuilder()
    : m_platform(PlatformType::OPENGL_GLFW),
      m_font_resolver(FontResolverType::FONTCONFIG),
      m_font_engine(FontEngineType::FREETYPE)
{
}

ApplicationBuilder &ApplicationBuilder::withPlatform(PlatformType platform)
{
    m_platform = platform;
    return *this;
}

ApplicationBuilder &ApplicationBuilder::withFontResolver(FontResolverType resolver)
{
    m_font_resolver = resolver;
    return *this;
}

ApplicationBuilder &ApplicationBuilder::withFontEngine(FontEngineType engine)
{
    m_font_engine = engine;
    return *this;
}

std::unique_ptr<Window> ApplicationBuilder::build()
{
    std::unique_ptr<Window> window;

    switch (m_platform)
    {
    case PlatformType::OPENGL_GLFW:
        window = std::make_unique<opengl::GLFWWindow>();
        break;
    }

    if (window)
    {
        window->setFontDependencies(
            createFontResolver(),
            createFontEngineFactory());
    }

    return window;
}

std::unique_ptr<domain::FontResolver> ApplicationBuilder::createFontResolver()
{
    switch (m_font_resolver)
    {
    case FontResolverType::FONTCONFIG:
        return std::make_unique<FontconfigResolver>();
    }

    return nullptr;
}

FontEngineFactory ApplicationBuilder::createFontEngineFactory()
{
    switch (m_font_engine)
    {
    case FontEngineType::FREETYPE:
    {
        auto freetype_library = std::make_shared<FreeTypeLibrary>();
        return FreeTypeFontEngine::createFactory(freetype_library);
    }
    }

    return nullptr;
}
