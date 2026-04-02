#ifndef DOMAIN_PORTS_WINDOW_HPP_INCLUDED
#define DOMAIN_PORTS_WINDOW_HPP_INCLUDED

#include <functional>
#include <string>

#include "domain/color.hpp"
#include "domain/eventfilters.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/observerlist.hpp"

namespace domain
{

struct ResizeEvent
{
    int width;
    int height;
};

struct CloseEvent
{
};

struct MaximizedChangedEvent
{
    bool maximized;
};

using WindowEvent = std::variant<ResizeEvent, KeyEvent, MouseMoveEvent, MouseButtonEvent, MouseScrollEvent, CloseEvent,
                                 MaximizedChangedEvent>;

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

    virtual EventFilterId installEventFilter(std::function<bool(const WindowEvent &)>) = 0;
    virtual void removeEventFilter(domain::EventFilterId id) = 0;

    virtual ObserverId onResize(std::function<void(const ResizeEvent &)> callback) = 0;
    virtual ObserverId onKeyInput(std::function<void(const KeyEvent &)> callback) = 0;
    virtual ObserverId onMouseMove(std::function<void(const MouseMoveEvent &)> callback) = 0;
    virtual ObserverId onMouseButton(std::function<void(const MouseButtonEvent &)> callback) = 0;
    virtual ObserverId onMouseScroll(std::function<void(const MouseScrollEvent &)> callback) = 0;
    virtual ObserverId onClose(std::function<void(const CloseEvent &)> callback) = 0;
    virtual ObserverId onMaximizedChanged(std::function<void(const MaximizedChangedEvent &)> callback) = 0;
    virtual void removeObserver(ObserverId id) = 0;

  private:
    bool m_needs_rerender = true;
};

} // namespace domain

#endif
