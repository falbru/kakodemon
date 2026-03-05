#include "applicationbuilder.hpp"
#include "adapters/fontconfig/fontconfigresolver.hpp"
#include "adapters/freetype/freetypefontengine.hpp"
#include "adapters/freetype/freetypelibrary.hpp"
#include "adapters/opengl/renderer.hpp"
#include "adapters/opengl/window.hpp"
#include "application/application.hpp"
#include "domain/ports/window.hpp"
#include "domain/fontmanager.hpp"
#include "domain/ports/renderer.hpp"
#include <memory>

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

std::unique_ptr<Application> ApplicationBuilder::build()
{
    std::unique_ptr<domain::Window> window;
    std::unique_ptr<domain::Renderer> renderer;
    domain::FontFactory font_factory;

    switch (m_platform)
    {
    case PlatformType::OPENGL_GLFW:
        window = std::make_unique<opengl::GLFWWindow>();
        renderer = std::make_unique<opengl::Renderer>();
        font_factory = opengl::Renderer::getFontFactory();
        break;
    }

    auto font_manager = std::make_unique<domain::FontManager>(createFontResolver(), createFontEngineFactory(), font_factory);
    return std::make_unique<Application>(std::move(window), std::move(renderer), std::move(font_manager));
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
