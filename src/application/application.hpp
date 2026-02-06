#ifndef APPLICATION_APPLICATION_HPP_INCLUDED
#define APPLICATION_APPLICATION_HPP_INCLUDED

#include <functional>
#include <memory>

#include "application/applicationconfig.hpp"
#include "application/cliconfig.hpp"
#include "application/controller/commandcontroller.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/focuscontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "application/controller/layoutcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/controller/mousecontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/searchmenu.hpp"
#include "application/view/statusbar.hpp"
#include "domain/fontmanager.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/ports/renderer.hpp"

class Window;

class Application
{
  public:
    Application();
    ~Application();

    void init(Window *window, const CliConfig &cli_config, ApplicationConfig &app_config);
    void run();

    void markDirty();

  private:
    void updateControllers();
    void renderControllers();

    Window *m_window;
    bool m_running = true;
    bool m_needs_render = true;

    domain::Renderer *m_renderer;
    domain::FontManager *m_font_manager;

    std::string m_kakodemon_id;
    ApplicationConfig *m_app_config;

    std::unique_ptr<CommandInterface> m_command_interface;

    std::unique_ptr<domain::KakouneSession> m_kakoune_session;
    std::unique_ptr<ClientManager> m_client_manager;
    std::unique_ptr<PaneLayout> m_pane_layout;
    KakouneClient *m_focused_client;

    std::unique_ptr<CommandController> m_command_controller;
    std::unique_ptr<EditorController> m_editor_controller;
    std::unique_ptr<FocusController> m_focus_controller;
    std::unique_ptr<LayoutController> m_layout_controller;
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
