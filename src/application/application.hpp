#ifndef APPLICATION_APPLICATION_HPP_INCLUDED
#define APPLICATION_APPLICATION_HPP_INCLUDED

#include <memory>

#include "adapters/opengl/opengl.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/controller/mousecontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/searchmenu.hpp"
#include "application/view/statusbar.hpp"
#include "domain/color.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/renderer.hpp"
#include "kakoune/kakouneclientprocess.hpp"

class Application
{
  public:
    Application();
    virtual ~Application();

    virtual void init();
    virtual void run() = 0;

    void setClearColor(domain::Color color);
    virtual void setCursor(domain::Cursor cursor);

    void onWindowResize(int width, int height);
    void onKeyInput(domain::KeyEvent event);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);
    void onMouseScroll(double offset);

  protected:
    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;

    std::unique_ptr<domain::Renderer> m_renderer;

    void updateControllers();
    void renderControllers();

    domain::Color m_clear_color;
    domain::Cursor m_cursor;

    float m_mouse_x = 0.0f;
    float m_mouse_y = 0.0f;

    std::unique_ptr<KakouneClient> m_kakoune_client;
    std::unique_ptr<UIOptions> m_ui_options;

    std::unique_ptr<EditorController> m_editor_controller;
    std::unique_ptr<InputController> m_input_controller;
    std::unique_ptr<MouseController> m_mouse_controller;
    std::unique_ptr<MenuController> m_menu_controller;
    std::unique_ptr<InfoBoxController> m_info_box_controller;

    std::unique_ptr<KakouneContentView> m_kakoune_content_view;
    std::unique_ptr<StatusBarView> m_status_bar;
    std::unique_ptr<PromptMenuView> m_prompt_menu;
    std::unique_ptr<InlineMenuView> m_inline_menu;
    std::unique_ptr<SearchMenuView> m_search_menu;
    std::unique_ptr<InfoBoxView> m_info_box;
};

#endif
