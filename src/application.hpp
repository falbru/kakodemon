#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <memory>

#include "controller/editorcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "opengl.hpp"
#include "view/kakounecontentview.hpp"

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

    GLFWwindow *m_window;

    std::shared_ptr<EditorController> m_editor_controller;
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<KakouneContentView> m_kakoune_content_view;

    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;
};

#endif
