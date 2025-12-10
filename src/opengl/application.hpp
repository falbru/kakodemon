#ifndef OPENGL_APPLICATION_HPP_INCLUDED
#define OPENGL_APPLICATION_HPP_INCLUDED

#include "../application.hpp"
#include "opengl.hpp"

namespace opengl
{

class GLFWApplication : public Application
{
  public:
    GLFWApplication();
    ~GLFWApplication();

    void init() override;
    void run() override;

  private:
    void onGLFWKeyInput(int key, int scancode, int action, int mods);
    void onGLFWCharacterInsert(unsigned int codepoint, int mods);

    std::optional<core::KeyEvent> glfwSpecialKeyToKeyEvent(int key, int mods);
    core::KeyEvent glfwCharToKeyEvent(unsigned int codepoint, int mods);

    GLFWwindow *m_window;
};

}; // namespace opengl

#endif
