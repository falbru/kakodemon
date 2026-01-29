#ifndef APPLICATION_APPLICATION_HPP_INCLUDED
#define APPLICATION_APPLICATION_HPP_INCLUDED

#include <functional>
#include <memory>

#include "adapters/opengl/opengl.hpp"
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
#include "application/model/kakouneclient.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/searchmenu.hpp"
#include "application/view/statusbar.hpp"
#include "domain/fontmanager.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/fontengine.hpp"
#include "domain/ports/fontresolver.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/uioptions.hpp"

using FontEngineFactory = std::function<std::unique_ptr<domain::FontEngine>(const domain::FontMatch &)>;

class Application
{
  public:
    Application();
    virtual ~Application();

    void setFontDependencies(std::unique_ptr<domain::FontResolver> resolver, FontEngineFactory engine_factory);

    virtual void init(const CliConfig &cli_config, ApplicationConfig &app_config);
    virtual void run() = 0;

    void setClearColor(domain::RGBAColor color);
    virtual void setCursor(domain::Cursor cursor);
    virtual void setWindowTitle(const std::string &title) = 0;

    void onWindowResize(int width, int height);
    void onKeyInput(domain::KeyEvent event);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);
    void onMouseScroll(double offset);

    void markDirty();
    bool needsRender() const;
    virtual void wakeEventLoop() = 0;
    virtual float getWidth() = 0;
    virtual float getHeight() = 0;

  protected:
    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 480;

    void createKakouneClient(std::optional<std::string> startup_command, std::vector<std::string> file_arguments);

    std::unique_ptr<domain::Renderer> m_renderer;
    std::unique_ptr<domain::FontManager> m_font_manager;

    std::unique_ptr<domain::FontResolver> m_font_resolver;
    FontEngineFactory m_font_engine_factory;

    void updateControllers();
    void renderControllers();

    domain::RGBAColor m_clear_color;
    domain::Cursor m_cursor;

    float m_mouse_x = 0.0f;
    float m_mouse_y = 0.0f;

    bool m_needs_render = true;

    std::string m_kakodemon_id;
    ApplicationConfig *m_app_config;

    std::unique_ptr<CommandInterface> m_command_interface;

    std::unique_ptr<domain::KakouneSession> m_kakoune_session;
    std::vector<std::unique_ptr<KakouneClient>> m_kakoune_clients;
    KakouneClient *m_focused_client;
    std::unique_ptr<domain::UIOptions> m_ui_options;

    std::unique_ptr<CommandController> m_command_controller;
    std::unique_ptr<EditorController> m_editor_controller;
    std::unique_ptr<FocusController> m_focus_controller;
    std::unique_ptr<InputController> m_input_controller;
    std::unique_ptr<LayoutController> m_layout_controller;
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
