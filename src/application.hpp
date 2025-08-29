#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <memory>

#include "controller/editorcontroller.hpp"
#include "controller/inputcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "opengl/opengl.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"
#include "view/statusbar.hpp"

class Application
{
  public:
    Application();
    ~Application();

    void init();
    void run();

  private:
    void initGLFW();
    void initMVC();

    void onWindowResize(int width, int height);
    void onKeyInput(int key, int scancode, int action, int mods);
    void onCharacterInsert(unsigned int codepoint, int mods);

    std::optional<KeyEvent> glfwSpecialKeyToKeyEvent(int key, int mods);
    KeyEvent glfwCharToKeyEvent(unsigned int codepoint, int mods);

    GLFWwindow *m_window;

    std::shared_ptr<EditorController> m_editor_controller;
    std::shared_ptr<InputController> m_input_controller;
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneClientProcess> m_kakoune_process;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<StatusBarView> m_status_bar;

    std::shared_ptr<opengl::Renderer> m_renderer;

    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;
};

#endif
