#ifndef APPLICATION_APPLICATION_HPP_INCLUDED
#define APPLICATION_APPLICATION_HPP_INCLUDED

#include <memory>

#include "application/applicationconfig.hpp"
#include "application/cliconfig.hpp"
#include "application/controller/commandcontroller.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/focuscontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "application/controller/layoutcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "application/scene.hpp"
#include "application/view/infobox.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/multistyledmenu.hpp"
#include "application/view/paneborderview.hpp"
#include "application/view/statusbar.hpp"
#include "domain/fontmanager.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/ports/window.hpp"

class Application
{
  public:
    Application(std::unique_ptr<domain::Window> window, std::unique_ptr<domain::Renderer> renderer,
                std::unique_ptr<domain::FontManager> font_manager);
    ~Application();

    void init(const CliConfig &cli_config, const ApplicationConfig &app_config);
    void run();

  private:
    void updateControllers();
    void renderControllers();

    std::unique_ptr<domain::Window> m_window;
    bool m_running = true;

    std::unique_ptr<domain::Renderer> m_renderer;
    std::unique_ptr<domain::FontManager> m_font_manager;

    std::string m_kakodemon_id;
    ApplicationConfig m_app_config;

    std::unique_ptr<CommandInterface> m_command_interface;

    std::unique_ptr<domain::KakouneSession> m_kakoune_session;
    std::unique_ptr<ClientManager> m_client_manager;
    std::unique_ptr<FocusedClientStack> m_focused_client_stack;
    std::unique_ptr<PaneLayout> m_pane_layout;

    std::unique_ptr<CommandController> m_command_controller;
    std::unique_ptr<EditorController> m_editor_controller;
    std::unique_ptr<FocusController> m_focus_controller;
    std::unique_ptr<LayoutController> m_layout_controller;
    std::unique_ptr<InputController> m_input_controller;
    std::unique_ptr<Scene> m_scene;

    std::unique_ptr<KakouneContentView> m_kakoune_content_view;
    std::unique_ptr<StatusBarView> m_status_bar;
    std::unique_ptr<MultiStyledMenuView> m_multi_styled_menu;
    std::unique_ptr<InfoBoxView> m_info_box;
    std::unique_ptr<PaneBorderView> m_pane_border_view;
};

#endif
