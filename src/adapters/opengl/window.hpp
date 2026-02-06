#ifndef OPENGL_WINDOW_HPP_INCLUDED
#define OPENGL_WINDOW_HPP_INCLUDED

#include "application/window.hpp"
#include "opengl.hpp"
#include <map>
#include <optional>

namespace opengl
{

class GLFWWindow : public Window
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

    domain::Renderer *getRenderer() override;
    domain::FontManager *getFontManager() override;

    void setFontDependencies(std::unique_ptr<domain::FontResolver> resolver, FontEngineFactory engine_factory) override;

    ObserverId onResize(std::function<void(int, int)> callback) override;
    ObserverId onKeyInput(std::function<void(domain::KeyEvent)> callback) override;
    ObserverId onMouseMove(std::function<void(float, float)> callback) override;
    ObserverId onMouseButton(std::function<void(domain::MouseButtonEvent)> callback) override;
    ObserverId onMouseScroll(std::function<void(double)> callback) override;
    ObserverId onClose(std::function<void()> callback) override;
    ObserverId onMaximizedChanged(std::function<void(bool)> callback) override;
    void removeObserver(ObserverId id) override;

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

    float m_cursor_x = 0.0f;
    float m_cursor_y = 0.0f;

    domain::RGBAColor m_clear_color = {};

    std::unique_ptr<domain::Renderer> m_renderer;
    std::unique_ptr<domain::FontManager> m_font_manager;

    std::unique_ptr<domain::FontResolver> m_font_resolver;
    FontEngineFactory m_font_engine_factory;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void(int, int)>> m_resize_observers;
    std::map<ObserverId, std::function<void(domain::KeyEvent)>> m_key_input_observers;
    std::map<ObserverId, std::function<void(float, float)>> m_mouse_move_observers;
    std::map<ObserverId, std::function<void(domain::MouseButtonEvent)>> m_mouse_button_observers;
    std::map<ObserverId, std::function<void(double)>> m_mouse_scroll_observers;
    std::map<ObserverId, std::function<void()>> m_close_observers;
    std::map<ObserverId, std::function<void(bool)>> m_maximized_changed_observers;
};

} // namespace opengl

#endif
