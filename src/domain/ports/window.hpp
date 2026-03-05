#ifndef DOMAIN_PORTS_WINDOW_HPP_INCLUDED
#define DOMAIN_PORTS_WINDOW_HPP_INCLUDED

#include <functional>
#include <string>

#include "domain/color.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/observerid.hpp"

namespace domain
{

class Window
{
  public:
    virtual ~Window() = default;

    virtual void init(bool maximized) = 0;

    virtual void waitEvents() = 0;
    virtual void wakeEventLoop() = 0;

    virtual void renderBegin() = 0;
    virtual void renderEnd() = 0;

    void setNeedsRerender();
    bool needsRerender() const;
    void clearRerender();
    virtual void setClearColor(RGBAColor color) = 0;

    virtual void setTitle(const std::string &title) = 0;
    virtual void setCursor(Cursor cursor) = 0;
    virtual float getWidth() = 0;
    virtual float getHeight() = 0;

    virtual ObserverId onResize(std::function<void(int, int)> callback) = 0;
    virtual ObserverId onKeyInput(std::function<void(KeyEvent)> callback) = 0;
    virtual ObserverId onMouseMove(std::function<void(float, float)> callback) = 0;
    virtual ObserverId onMouseButton(std::function<void(MouseButtonEvent)> callback) = 0;
    virtual ObserverId onMouseScroll(std::function<void(double)> callback) = 0;
    virtual ObserverId onClose(std::function<void()> callback) = 0;
    virtual ObserverId onMaximizedChanged(std::function<void(bool)> callback) = 0;
    virtual void removeObserver(ObserverId id) = 0;

  private:
    bool m_needs_rerender = true;
};

} // namespace domain

#endif
