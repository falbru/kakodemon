#include "application.hpp"
#include "adapters/kakoune/kakouneclientprocess.hpp"
#include "adapters/kakoune/localsession.hpp"
#include "adapters/namedpipe/namedpipecommandinterface.hpp"
#include "application/cliconfig.hpp"
#include "application/controller/commandcontroller.hpp"
#include "application/controller/layoutcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/ports/window.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "domain/fontmanager.hpp"
#include "domain/ports/commandinterface.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/uioptions.hpp"
#include "application/view/infobox.hpp"
#include "application/view/multistyledmenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "adapters/kakoune/remotesession.hpp"
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

Application::Application(std::unique_ptr<domain::Window> window, std::unique_ptr<domain::Renderer> renderer, std::unique_ptr<domain::FontManager> font_manager)
    : m_window(std::move(window)), m_renderer(std::move(renderer)), m_font_manager(std::move(font_manager))
{
}

void Application::init(const CliConfig &cli_config, const ApplicationConfig &app_config)
{
    m_app_config = app_config;

    m_window->init(m_app_config.maximized);
    m_renderer->init(m_window->getWidth(), m_window->getHeight());
    m_window->onResize([this](int width, int height) { m_renderer->onWindowResize(width, height); });

    m_kakodemon_id = generateId();
    setenv("KAKOD_ID", m_kakodemon_id.c_str(), 1);

    m_command_interface = std::make_unique<NamedPipeCommandInterface>(m_kakodemon_id, PipeMode::Receive);
    m_command_interface->onCommandReceived([this](const Command&) {
        m_window->wakeEventLoop();
    });
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
    m_focused_client_stack = std::make_unique<FocusedClientStack>();

    m_client_manager->onClientAdded([this](KakouneClient* client) {
        client->interface->onRefresh([this](bool) { m_window->wakeEventLoop(); });
        client->interface->onExit([this, client]() {
            m_client_manager->removeClient(client);
        });
    });

    m_client_manager->onClientRemoved([this](KakouneClient*) {
        if (m_client_manager->clients().empty()) {
            m_running = false;
            m_window->wakeEventLoop();
        }
    });

    m_window->onClose([this]() { m_running = false; });

    m_window->onMaximizedChanged([this](bool maximized) {
        m_app_config.maximized = maximized;
        saveApplicationConfig(m_app_config);
    });

    m_pane_layout = std::make_unique<PaneLayout>();

    m_command_controller = std::make_unique<CommandController>();
    m_editor_controller = std::make_unique<EditorController>();
    m_focus_controller = std::make_unique<FocusController>();
    m_input_controller = std::make_unique<InputController>();
    m_layout_controller = std::make_unique<LayoutController>();
    m_scene = std::make_unique<Scene>();

    m_kakoune_content_view = std::make_unique<KakouneContentView>();
    m_status_bar = std::make_unique<StatusBarView>();
    m_multi_styled_menu = std::make_unique<MultiStyledMenuView>();
    m_info_box = std::make_unique<InfoBoxView>();
    m_pane_border_view = std::make_unique<PaneBorderView>();

    m_kakoune_content_view->init(m_renderer.get());
    m_status_bar->init(m_renderer.get());
    m_multi_styled_menu->init(m_renderer.get(), m_kakoune_content_view.get());
    m_info_box->init(m_renderer.get(), m_multi_styled_menu.get(), m_kakoune_content_view.get(), m_status_bar.get());
    m_pane_border_view->init(m_renderer.get());

    m_multi_styled_menu->onMouseButton([this](int item_index) {
        m_focused_client_stack->focused()->interface->selectMenuItem(item_index); // TODO move to controller
    });

    m_pane_layout->init(m_client_manager.get());

    m_command_controller->init(m_command_interface.get(), m_client_manager.get(), m_kakoune_session.get(), m_window.get());
    m_input_controller->init(m_focused_client_stack.get(), m_window.get());
    m_focus_controller->init(m_focused_client_stack.get(), m_client_manager.get(), m_pane_layout.get(), m_window.get());
    m_editor_controller->init(m_client_manager.get(), m_pane_layout.get(), m_kakoune_content_view.get(), m_status_bar.get(), m_font_manager.get(), m_window.get(), m_multi_styled_menu.get());
    m_scene->init(m_client_manager.get(), m_focused_client_stack.get(), m_pane_layout.get(), m_kakoune_content_view.get(),
                  m_status_bar.get(), m_multi_styled_menu.get(), m_info_box.get(), m_font_manager.get(), m_window.get(),
                  m_pane_border_view.get());
    m_layout_controller->init(m_pane_layout.get(), m_client_manager.get(), m_window.get());

    m_client_manager->setDefaultUIOptions(domain::getDefaultUIOptions(m_font_manager.get()));

    m_client_manager->createClient(cli_config.startup_command, cli_config.session_type == SessionType::Remote ? cli_config.file_arguments : std::vector<std::string>{});

    float initial_width = m_window->getWidth();
    float initial_height = m_window->getHeight();
    m_pane_layout->setBounds(domain::Rectangle{0, 0, initial_width, initial_height});
    m_pane_layout->arrange();
}

Application::~Application()
{
}

void Application::run() {
    while (m_running) {
        m_window->waitEvents();

        updateControllers();

        if (m_window->needsRerender()) {
            m_window->renderBegin();
            renderControllers();
            m_window->renderEnd();
            m_window->clearRerender();
        }
    }
}

void Application::updateControllers()
{
    KakouneClientProcess::processPendingExits();
    m_command_controller->update();
    m_input_controller->update();
    m_editor_controller->update();
}

void Application::renderControllers()
{
    m_scene->render();
}

