#ifndef APPLICATION_WINDOW_HPP_INCLUDED
#define APPLICATION_WINDOW_HPP_INCLUDED

#include <functional>
#include <memory>
#include <string>

#include "application/observerid.hpp"
#include "domain/color.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/ports/fontresolver.hpp"

using FontEngineFactory = std::function<std::unique_ptr<domain::FontEngine>(const domain::FontMatch &)>;

namespace domain
{
class Renderer;
class FontManager;
} // namespace domain

class Window
{
  public:
    virtual ~Window() = default;

    virtual void init(bool maximized) = 0;

    virtual void waitEvents() = 0;
    virtual void wakeEventLoop() = 0;

    virtual void renderBegin() = 0;
    virtual void renderEnd() = 0;
    virtual void setClearColor(domain::RGBAColor color) = 0;

    virtual void setTitle(const std::string &title) = 0;
    virtual void setCursor(domain::Cursor cursor) = 0;
    virtual float getWidth() = 0;
    virtual float getHeight() = 0;

    virtual domain::Renderer *getRenderer() = 0;
    virtual domain::FontManager *getFontManager() = 0;

    virtual void setFontDependencies(std::unique_ptr<domain::FontResolver> resolver,
                                     FontEngineFactory engine_factory) = 0;

    virtual ObserverId onResize(std::function<void(int, int)> callback) = 0;
    virtual ObserverId onKeyInput(std::function<void(domain::KeyEvent)> callback) = 0;
    virtual ObserverId onMouseMove(std::function<void(float, float)> callback) = 0;
    virtual ObserverId onMouseButton(std::function<void(domain::MouseButtonEvent)> callback) = 0;
    virtual ObserverId onMouseScroll(std::function<void(double)> callback) = 0;
    virtual ObserverId onClose(std::function<void()> callback) = 0;
    virtual ObserverId onMaximizedChanged(std::function<void(bool)> callback) = 0;
    virtual void removeObserver(ObserverId id) = 0;
};

#endif
