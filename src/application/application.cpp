#include "application.hpp"
#include "adapters/kakoune/kakouneclientprocess.hpp"
#include "adapters/kakoune/localsession.hpp"
#include "adapters/namedpipe/namedpipecommandinterface.hpp"
#include "application/cliconfig.hpp"
#include "application/controller/commandcontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/layoutcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/controller/mousecontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/view/searchmenu.hpp"
#include "domain/color.hpp"
#include "domain/keys.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "domain/uioptions.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "adapters/kakoune/remotesession.hpp"
#include "domain/mouse.hpp"
#include <cstdlib>
#include <memory>
#include <optional>
#include <random>

namespace {

std::string generateId(size_t length = 8) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string id;
    id.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        id += charset[dist(gen)];
    }
    return id;
}

}

Application::Application()
{
}

Application::~Application()
{
}

void Application::setFontDependencies(
    std::unique_ptr<domain::FontResolver> resolver,
    FontEngineFactory engine_factory)
{
    m_font_resolver = std::move(resolver);
    m_font_engine_factory = engine_factory;
}

void Application::init(const CliConfig &cli_config, ApplicationConfig &app_config)
{
    m_app_config = &app_config;
    m_kakodemon_id = generateId();
    setenv("KAKOD_ID", m_kakodemon_id.c_str(), 1);

    m_command_interface = std::make_unique<NamedPipeCommandInterface>(m_kakodemon_id, PipeMode::Receive);
    m_command_interface->setWakeEventLoopCallback([this]() { wakeEventLoop(); });
    m_command_interface->init();

    if (cli_config.session_type == SessionType::Remote)
    {
        m_kakoune_session = std::make_unique<RemoteSession>(cli_config.session_id);
    }else {
        auto local_session = std::make_unique<LocalSession>(cli_config.session_id);
        local_session->start(cli_config.file_arguments, cli_config.no_config);
        m_kakoune_session = std::move(local_session);
    }

    m_client_manager = std::make_unique<ClientManager>(m_kakoune_session.get());

    m_client_manager->onClientAdded([this](KakouneClient* client) {
        client->interface->onRefresh([this](bool) { wakeEventLoop(); });
        client->interface->onExit([this, client]() {
            m_client_manager->removeClient(client);
        });
    });

    m_pane_layout = std::make_unique<PaneLayout>();

    m_command_controller = std::make_unique<CommandController>();
    m_editor_controller = std::make_unique<EditorController>();
    m_focus_controller = std::make_unique<FocusController>();
    m_input_controller = std::make_unique<InputController>();
    m_mouse_controller = std::make_unique<MouseController>();
    m_menu_controller = std::make_unique<MenuController>();
    m_info_box_controller = std::make_unique<InfoBoxController>();
    m_layout_controller = std::make_unique<LayoutController>();

    m_kakoune_content_view = std::make_unique<KakouneContentView>();
    m_status_bar = std::make_unique<StatusBarView>();
    m_prompt_menu = std::make_unique<PromptMenuView>();
    m_inline_menu = std::make_unique<InlineMenuView>();
    m_search_menu = std::make_unique<SearchMenuView>();
    m_info_box = std::make_unique<InfoBoxView>();

    m_kakoune_content_view->init(m_renderer.get());
    m_status_bar->init(m_renderer.get());
    m_prompt_menu->init(m_renderer.get(), m_kakoune_content_view.get());
    m_inline_menu->init(m_renderer.get(), m_kakoune_content_view.get());
    m_search_menu->init(m_renderer.get());
    m_info_box->init(m_renderer.get(), m_menu_controller.get(), m_kakoune_content_view.get(), m_status_bar.get());

    m_pane_layout->init(m_client_manager.get());

    m_command_controller->init(m_command_interface.get(), m_client_manager.get(), m_kakoune_session.get(), [this](const std::string &title) { setWindowTitle(title); });
    m_input_controller->init(&m_focused_client);
    m_focus_controller->init(&m_focused_client, m_client_manager.get(), m_pane_layout.get());
    m_menu_controller->init(&m_focused_client, m_pane_layout.get(), m_editor_controller.get(), m_font_manager.get(), m_prompt_menu.get(), m_inline_menu.get(), m_search_menu.get(), [this]() { markDirty(); });
    m_editor_controller->init(m_client_manager.get(), &m_focused_client, m_pane_layout.get(), m_kakoune_content_view.get(), m_status_bar.get(), m_font_manager.get(), [&](domain::RGBAColor color) { setClearColor(color); }, m_menu_controller.get());
    m_info_box_controller->init(&m_focused_client, m_pane_layout.get(), m_editor_controller.get(), m_font_manager.get(), m_info_box.get(), [this]() { markDirty(); });
    m_mouse_controller->init(&m_focused_client, m_editor_controller.get(), m_menu_controller.get(), m_info_box_controller.get());
    m_layout_controller->init(m_pane_layout.get(), m_client_manager.get());

    m_client_manager->setDefaultUIOptions(domain::getDefaultUIOptions(m_font_manager.get()));

    m_client_manager->createClient(cli_config.startup_command, cli_config.session_type == SessionType::Remote ? cli_config.file_arguments : std::vector<std::string>{});
}

void Application::updateControllers()
{
    KakouneClientProcess::processPendingExits();
    m_command_controller->update();
    m_input_controller->update();
    bool state_updated = m_editor_controller->update();
    if (state_updated) {
        markDirty();
    }
}

void Application::renderControllers()
{
    m_editor_controller->render();
    m_menu_controller->render();
    m_info_box_controller->render();
}

void Application::onWindowResize(int width, int height)
{
    m_renderer->onWindowResize(width, height);
    m_layout_controller->onWindowResize(width, height);
    m_editor_controller->onWindowResize(width, height);
    markDirty();
}

void Application::onMouseScroll(double offset)
{
    m_mouse_controller->onMouseScroll(offset, m_mouse_x, m_mouse_y);
}

void Application::onMouseMove(float x, float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
    m_focus_controller->onMouseMove(x, y);
    domain::MouseMoveResult result = m_mouse_controller->onMouseMove(x, y);

    if (result.cursor.has_value()) {
        setCursor(result.cursor.value());
        markDirty();
    }
}

void Application::onMouseButton(domain::MouseButtonEvent event)
{
    m_focus_controller->onMouseButton(event);
    m_mouse_controller->onMouseButton(event);
    markDirty();
}

void Application::onKeyInput(domain::KeyEvent event)
{
    m_input_controller->onKeyInput(event);
    markDirty();
}

void Application::setClearColor(domain::RGBAColor color) {
    m_clear_color = color;
}

void Application::setCursor(domain::Cursor cursor) {
    m_cursor = cursor;
}

void Application::markDirty() {
    m_needs_render = true;
}

bool Application::needsRender() const {
    return m_needs_render;
}
