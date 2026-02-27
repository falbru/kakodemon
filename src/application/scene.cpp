#include "application/scene.hpp"
#include "application/view/rendercontext.hpp"
#include "application/window.hpp"
#include "domain/editor.hpp"
#include "domain/mouse.hpp"

Scene::Scene() {}

void Scene::init(ClientManager *client_manager, KakouneClient **focused_client, PaneLayout *pane_layout,
                 KakouneContentView *content_view, StatusBarView *status_bar_view, PromptMenuView *prompt_menu_view,
                 InlineMenuView *inline_menu_view, SearchMenuView *search_menu_view, InfoBoxView *info_box_view,
                 MenuController *menu_controller, InfoBoxController *info_box_controller,
                 domain::FontManager *font_manager, Window *window)
{
    m_focused_client = focused_client;
    m_pane_layout = pane_layout;
    m_content_view = content_view;
    m_status_bar_view = status_bar_view;
    m_prompt_menu_view = prompt_menu_view;
    m_inline_menu_view = inline_menu_view;
    m_search_menu_view = search_menu_view;
    m_info_box_view = info_box_view;
    m_menu_controller = menu_controller;
    m_info_box_controller = info_box_controller;
    m_font_manager = font_manager;
    m_window = window;

    client_manager->onClientRemoved([this](KakouneClient *client) {
        if (m_active_mouse_client == client) {
            m_active_mouse_client = nullptr;
        }
    });

    window->onMouseMove([this](float x, float y) {
        m_mouse_x = x;
        m_mouse_y = y;
        domain::MouseMoveResult result = onMouseMove(x, y);
        if (result.cursor.has_value()) {
            m_window->setCursor(result.cursor.value());
            m_window->setNeedsRerender();
        }
    });

    window->onMouseButton([this](domain::MouseButtonEvent event) {
        onMouseButton(event);
        m_window->setNeedsRerender();
    });

    window->onMouseScroll([this](double offset) {
        onMouseScroll(offset);
        m_window->setNeedsRerender();
    });
}

void Scene::render()
{
    for (const auto &pane : m_pane_layout->getPanes()) {
        auto *client = pane.client;
        const auto &bounds = pane.bounds;

        RenderContext render_context = {
            m_font_manager,
            client->state.default_face,
            client->uiOptions(),
            bounds.width,
            bounds.height,
        };

        m_content_view->render(render_context, client->state.content, client->state.default_face, bounds);
        m_status_bar_view->render(render_context, client->status_line_state, client->state.mode_line,
                                  client->state.cursor_position, bounds);
    }

    if (!*m_focused_client) return;
    auto *focused = *m_focused_client;

    RenderContext focused_context = {
        m_font_manager,
        focused->state.default_face,
        focused->uiOptions(),
        static_cast<float>(m_window->getWidth()),
        static_cast<float>(m_window->getHeight()),
    };

    if (focused->state.info_box.has_value() &&
        (focused->state.info_box->title.size() > 0 || focused->state.info_box->content.size() > 0)) // TODO if content and title size == 0, it should just be a nullopt
    {
        Pane *pane = m_pane_layout->findPaneForClient(focused);
        if (pane) {
            m_info_box_view->render(focused_context, focused->info_box_state, *focused->state.info_box,
                                    focused->state.cursor_position, pane->bounds);
        }
    }

    if (focused->state.menu.has_value()) {
        int cursor_column = -1;
        if (std::holds_alternative<domain::StatusLinePosition>(focused->state.cursor_position)) {
            cursor_column = std::get<domain::StatusLinePosition>(focused->state.cursor_position).column;
        }

        Pane *pane = m_pane_layout->findPaneForClient(focused);

        switch (focused->state.menu->getStyle()) {
            case domain::MenuStyle::INLINE:
                if (pane) {
                    m_inline_menu_view->render(focused_context, focused->inline_menu_state,
                                               *focused->state.menu, pane->bounds);
                }
                break;
            case domain::MenuStyle::PROMPT:
                m_prompt_menu_view->render(focused_context, focused->prompt_menu_state,
                                           *focused->state.menu, cursor_column);
                break;
            case domain::MenuStyle::SEARCH:
                m_search_menu_view->render(focused_context, focused->search_menu_state,
                                           *focused->state.menu, cursor_column);
                break;
        }
    }
}

bool Scene::hitTestMenu(float x, float y) const
{
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) return false;
    return x >= m_menu_controller->x() && x <= m_menu_controller->x() + m_menu_controller->width() &&
           y >= m_menu_controller->y() && y <= m_menu_controller->y() + m_menu_controller->height();
}

bool Scene::hitTestInfoBox(float x, float y) const
{
    if (!*m_focused_client || !(*m_focused_client)->state.info_box.has_value()) return false;
    if ((*m_focused_client)->state.info_box->title.size() == 0 &&
        (*m_focused_client)->state.info_box->content.size() == 0) return false;
    return x >= m_info_box_view->x() && x <= m_info_box_view->x() + m_info_box_view->width() &&
           y >= m_info_box_view->y() && y <= m_info_box_view->y() + m_info_box_view->height();
}

domain::MouseMoveResult Scene::onMouseMove(float x, float y)
{
    Pane* pane = m_active_mouse_client ? m_pane_layout->findPaneForClient(m_active_mouse_client) : m_pane_layout->findPaneAt(x, y);
    if (pane) {
        m_content_view->handleMouseMove(pane->client, x, y, pane->bounds);
        return domain::MouseMoveResult{domain::Cursor::IBEAM};
    }

    domain::MouseMoveResult menu_result = m_menu_controller->handleMouseMove(x, y);
    if (menu_result.cursor.has_value()) return menu_result;

    if (hitTestInfoBox(x, y)) return domain::MouseMoveResult{domain::Cursor::DEFAULT};

    Pane *hover_pane = m_pane_layout->findPaneAt(x, y);
    if (!hover_pane) return domain::MouseMoveResult{domain::Cursor::DEFAULT};

    float status_bar_height = m_status_bar_view->height(hover_pane->client->uiOptions().font_statusbar);
    if (y - hover_pane->bounds.y >= hover_pane->bounds.height - status_bar_height) {
        return domain::MouseMoveResult{domain::Cursor::DEFAULT};
    }

    return domain::MouseMoveResult{domain::Cursor::IBEAM};
}

void Scene::onMouseButton(domain::MouseButtonEvent event)
{
    if (event.action == domain::MouseButtonAction::PRESS) {
        bool handled_by_menu = m_menu_controller->handleMouseButton(event);
        if (handled_by_menu) return;

        if (hitTestInfoBox(event.x, event.y)) return;

        Pane *pane = m_pane_layout->findPaneAt(event.x, event.y);
        if (!pane) return;

        float status_bar_height = m_status_bar_view->height(pane->client->uiOptions().font_statusbar);
        if (event.y - pane->bounds.y >= pane->bounds.height - status_bar_height) return;

        m_content_view->handleMouseButton(pane->client, event, pane->bounds);
        m_active_mouse_client = pane->client;
    } else {

        if (m_active_mouse_client) {
            Pane *pane = m_pane_layout->findPaneForClient(m_active_mouse_client);
            if (pane) {
                m_content_view->handleMouseButton(m_active_mouse_client, event, pane->bounds);
            }
        }
    }
}

void Scene::onMouseScroll(double offset)
{
    m_scroll_accumulator += offset * m_scroll_speed;

    int scroll_amount = 0;
    if (m_scroll_accumulator >= 1.0) {
        scroll_amount = static_cast<int>(m_scroll_accumulator);
        m_scroll_accumulator -= scroll_amount;
    } else if (m_scroll_accumulator <= -1.0) {
        scroll_amount = static_cast<int>(m_scroll_accumulator);
        m_scroll_accumulator -= scroll_amount;
    }

    if (scroll_amount == 0) return;

    if (hitTestMenu(m_mouse_x, m_mouse_y)) {
        m_menu_controller->handleMouseScroll(-scroll_amount);
        return;
    }

    if (hitTestInfoBox(m_mouse_x, m_mouse_y)) {
        m_info_box_controller->handleMouseScroll(-scroll_amount);
        return;
    }

    if (!*m_focused_client) return;
    Pane *pane = m_pane_layout->findPaneForClient(*m_focused_client);
    if (!pane) return;

    m_content_view->handleMouseScroll(*m_focused_client, m_mouse_x, m_mouse_y, pane->bounds, -scroll_amount);
}

void Scene::setScrollSpeed(double speed) { m_scroll_speed = speed; }
double Scene::getScrollSpeed() const { return m_scroll_speed; }
