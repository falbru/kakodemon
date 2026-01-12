#include "application.hpp"
#include "adapters/kakoune/localsession.hpp"
#include "application/cliconfig.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/controller/mousecontroller.hpp"
#include "application/view/searchmenu.hpp"
#include "domain/color.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/keys.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "domain/uioptions.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "adapters/kakoune/remotesession.hpp"
#include "domain/mouse.hpp"
#include <memory>

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

void Application::init(const CliConfig &config)
{
    std::unique_ptr<domain::KakouneSession> session;

    if (config.session_type == SessionType::Remote)
    {
        session = std::make_unique<RemoteSession>(config.session_id);
    }else {
        auto local_session = std::make_unique<LocalSession>(config.session_id);
        local_session->start();
        session = std::move(local_session);
    }

    auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*session, config.startup_command, config.file_arguments);
    interface->setWakeEventLoopCallback([this]() { wakeEventLoop(); });
    m_kakoune_client = std::make_unique<KakouneClient>(std::move(session), std::move(interface));
    m_ui_options = std::make_unique<domain::UIOptions>();

    m_editor_controller = std::make_unique<EditorController>();
    m_input_controller = std::make_unique<InputController>();
    m_mouse_controller = std::make_unique<MouseController>();
    m_menu_controller = std::make_unique<MenuController>();
    m_info_box_controller = std::make_unique<InfoBoxController>();

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
    m_info_box->init(m_renderer.get(), m_menu_controller.get(), m_kakoune_content_view.get());

    m_input_controller->init(m_kakoune_client.get());
    m_menu_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_font_manager.get(), m_prompt_menu.get(), m_inline_menu.get(), m_search_menu.get(), [this]() { markDirty(); });
    m_editor_controller->init(m_kakoune_client.get(), m_kakoune_content_view.get(), m_status_bar.get(), m_font_manager.get(), [&](domain::RGBAColor color) { setClearColor(color); }, m_menu_controller.get());
    m_info_box_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_font_manager.get(), m_info_box.get(), [this]() { markDirty(); });
    m_mouse_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_menu_controller.get(), m_info_box_controller.get());

    m_ui_options->font = m_font_manager->getFontFromName("monospace 14");
    loadBasicGlyphs(m_ui_options->font);
}

void Application::updateControllers()
{
    m_input_controller->update();
    bool state_updated = m_editor_controller->update(*m_ui_options.get());
    if (state_updated) {
        markDirty();
    }
    m_info_box_controller->update(*m_ui_options.get());
}

void Application::renderControllers()
{
    m_editor_controller->render(*m_ui_options.get());
    m_menu_controller->render(*m_ui_options.get());
    m_info_box_controller->render(*m_ui_options.get());
}

void Application::onWindowResize(int width, int height)
{
    m_renderer->onWindowResize(width, height);
    m_editor_controller->onWindowResize(width, height, *m_ui_options.get());
    markDirty();
}

void Application::onMouseScroll(double offset)
{
    m_mouse_controller->onMouseScroll(offset, m_mouse_x, m_mouse_y, m_ui_options.get());
}

void Application::onMouseMove(float x, float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
    domain::MouseMoveResult result = m_mouse_controller->onMouseMove(x, y, m_ui_options.get());

    if (result.cursor.has_value()) {
        setCursor(result.cursor.value());
        markDirty();
    }
}

void Application::onMouseButton(domain::MouseButtonEvent event)
{
    m_mouse_controller->onMouseButton(event, m_ui_options.get());
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
