#include "application.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/controller/mousecontroller.hpp"
#include "application/view/searchmenu.hpp"
#include "domain/color.hpp"
#include "domain/keys.hpp"
#include "application/controller/editorcontroller.hpp"
#include "application/controller/inputcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/infobox.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "domain/mouse.hpp"
#include <memory>

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{
    m_kakoune_client = std::make_unique<KakouneClient>();
    m_kakoune_client->process = std::make_unique<KakouneClientProcess>("default");
    m_ui_options = std::make_unique<UIOptions>();

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
    m_editor_controller->init(m_kakoune_client.get(), m_kakoune_content_view.get(), m_status_bar.get(), [&](domain::Color color) { setClearColor(color); });
    m_menu_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_prompt_menu.get(), m_inline_menu.get(), m_search_menu.get());
    m_info_box_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_info_box.get());
    m_mouse_controller->init(m_kakoune_client.get(), m_editor_controller.get(), m_menu_controller.get(), m_info_box_controller.get());

    m_kakoune_client->process->start();
}

void Application::updateControllers()
{
    m_editor_controller->update(*m_ui_options.get());
    m_menu_controller->update(*m_ui_options.get());
    m_info_box_controller->update(*m_ui_options.get());
}

void Application::onWindowResize(int width, int height)
{
    m_renderer->onWindowResize(width, height);
    m_editor_controller->onWindowResize(width, height, *m_ui_options.get());
}

void Application::onMouseMove(float x, float y)
{
    domain::MouseMoveResult result = m_mouse_controller->onMouseMove(x, y, m_ui_options.get());

    if (result.cursor.has_value()) {
        setCursor(result.cursor.value());
    }
}

void Application::onMouseButton(domain::MouseButtonEvent event)
{
    m_mouse_controller->onMouseButton(event, m_ui_options.get());
}

void Application::onKeyInput(domain::KeyEvent event)
{
    m_input_controller->onKeyInput(event);
}

void Application::setClearColor(domain::Color color) {
    m_clear_color = color;
}

void Application::setCursor(domain::Cursor cursor) {
    m_cursor = cursor;
}
