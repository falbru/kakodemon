#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <memory>

#include "controller/editorcontroller.hpp"
#include "controller/infoboxcontroller.hpp"
#include "controller/inputcontroller.hpp"
#include "controller/menucontroller.hpp"
#include "core/color.hpp"
#include "core/cursor.hpp"
#include "core/keys.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include "model/kakouneclient.hpp"
#include "model/uioptions.hpp"
#include "opengl/opengl.hpp"
#include "renderer.hpp"
#include "view/infobox.hpp"
#include "view/inlinemenu.hpp"
#include "view/kakounecontentview.hpp"
#include "view/promptmenu.hpp"
#include "view/statusbar.hpp"

class Application
{
  public:
    Application();
    virtual ~Application();

    virtual void init();
    virtual void run() = 0;

    void setClearColor(core::Color color);
    void setCursor(core::Cursor cursor);

    void onWindowResize(int width, int height);
    void onKeyInput(core::KeyEvent event);

  protected:
    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;

    std::unique_ptr<Renderer> m_renderer;

    void updateControllers();

    core::Color m_clear_color;
    core::Cursor m_cursor;

    std::unique_ptr<KakouneClient> m_kakoune_client;
    std::unique_ptr<UIOptions> m_ui_options;
    std::unique_ptr<KakouneClientProcess> m_kakoune_process;

    std::unique_ptr<EditorController> m_editor_controller;
    std::unique_ptr<InputController> m_input_controller;
    std::unique_ptr<MenuController> m_menu_controller;
    std::unique_ptr<InfoBoxController> m_info_box_controller;
    std::unique_ptr<KakouneContentView> m_kakoune_content_view;

    std::unique_ptr<StatusBarView> m_status_bar;
    std::unique_ptr<PromptMenuView> m_prompt_menu;
    std::unique_ptr<InlineMenuView> m_inline_menu;
    std::unique_ptr<InfoBoxView> m_info_box;
};

#endif
