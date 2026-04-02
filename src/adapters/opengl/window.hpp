#ifndef OPENGL_WINDOW_HPP_INCLUDED
#define OPENGL_WINDOW_HPP_INCLUDED

#include "domain/eventfilters.hpp"
#include "domain/mouse.hpp"
#include "domain/observerlist.hpp"
#include "domain/ports/window.hpp"
#include "opengl.hpp"
#include <optional>

namespace opengl
{

class GLFWWindow : public domain::Window
{
  public:
    GLFWWindow();
    ~GLFWWindow();

    void init(bool maximized) override;

    void waitEvents() override;
    void wakeEventLoop() override;

    void renderBegin() override;
    void renderEnd() override;
    void setClearColor(domain::RGBAColor color) override;

    void setTitle(const std::string &title) override;
    void setCursor(domain::Cursor cursor) override;
    float getWidth() override;
    float getHeight() override;

    domain::EventFilterId installEventFilter(std::function<bool(const domain::WindowEvent &)> event_filter) override;
    void removeEventFilter(domain::EventFilterId id) override;

    domain::ObserverId onResize(std::function<void(const domain::ResizeEvent &)> callback) override;
    domain::ObserverId onKeyInput(std::function<void(const domain::KeyEvent &)> callback) override;
    domain::ObserverId onMouseMove(std::function<void(const domain::MouseMoveEvent &)> callback) override;
    domain::ObserverId onMouseButton(std::function<void(const domain::MouseButtonEvent &)> callback) override;
    domain::ObserverId onMouseScroll(std::function<void(const domain::MouseScrollEvent &)> callback) override;
    domain::ObserverId onClose(std::function<void(const domain::CloseEvent &)> callback) override;
    domain::ObserverId onMaximizedChanged(std::function<void(const domain::MaximizedChangedEvent &)> callback) override;
    void removeObserver(domain::ObserverId id) override;

  private:
    void onGLFWKeyInput(int key, int scancode, int action, int mods);
    void onGLFWCharacterInsert(unsigned int codepoint, int mods);
    void onGLFWMouseMove(float cursor_x, float cursor_y);
    void onGLFWMouseButton(int button, int action, int mods);

    int toDomainModifiers(int mods);
    std::optional<domain::KeyEvent> glfwSpecialKeyToKeyEvent(int key, int mods);
    domain::KeyEvent glfwCharToKeyEvent(unsigned int codepoint, int mods);

    static constexpr int DEFAULT_WINDOW_WIDTH = 640;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 480;

    GLFWwindow *m_window = nullptr;
    GLFWcursor *m_cursor_ibeam = nullptr;
    GLFWcursor *m_cursor_pointer = nullptr;
    GLFWcursor *m_cursor_crosshair = nullptr;

    float m_cursor_x = 0.0f;
    float m_cursor_y = 0.0f;

    domain::RGBAColor m_clear_color = {};

    domain::EventFilters<domain::WindowEvent> m_event_filters;
    domain::ObserverList<const domain::ResizeEvent &> m_resize_observers;
    domain::ObserverList<const domain::KeyEvent &> m_key_input_observers;
    domain::ObserverList<const domain::MouseMoveEvent &> m_mouse_move_observers;
    domain::ObserverList<const domain::MouseButtonEvent &> m_mouse_button_observers;
    domain::ObserverList<const domain::MouseScrollEvent &> m_mouse_scroll_observers;
    domain::ObserverList<const domain::CloseEvent &> m_close_observers;
    domain::ObserverList<const domain::MaximizedChangedEvent &> m_maximized_changed_observers;
};

} // namespace opengl

#endif
