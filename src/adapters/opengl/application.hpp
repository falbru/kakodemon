#ifndef OPENGL_APPLICATION_HPP_INCLUDED
#define OPENGL_APPLICATION_HPP_INCLUDED

#include "adapters/freetype/freetypelibrary.hpp"
#include "application/application.hpp"
#include "application/cliconfig.hpp"
#include "opengl.hpp"
#include <chrono>
#include <memory>

namespace opengl
{

class GLFWApplication : public Application
{
  public:
    GLFWApplication();
    ~GLFWApplication();

    void init(const CliConfig &config) override;
    void run() override;

    void setCursor(domain::Cursor cursor) override;

  private:
    void onGLFWKeyInput(int key, int scancode, int action, int mods);
    void onGLFWCharacterInsert(unsigned int codepoint, int mods);
    void onGLFWMouseMove(float cursor_x, float cursor_y);
    void onGLFWMouseButton(int button, int action, int mods);

    int toDomainModifiers(int mods);

    std::optional<domain::KeyEvent> glfwSpecialKeyToKeyEvent(int key, int mods);
    domain::KeyEvent glfwCharToKeyEvent(unsigned int codepoint, int mods);

    GLFWwindow *m_window;

    GLFWcursor *m_cursor_ibeam;
    GLFWcursor *m_cursor_pointer;

    float m_cursor_x;
    float m_cursor_y;

    std::shared_ptr<FreeTypeLibrary> m_freetype_library;

    static constexpr double TARGET_FRAME_TIME_MS = 8;
};

}; // namespace opengl

#endif
