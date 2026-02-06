#include "menucontroller.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/editor.hpp"
#include "domain/mouse.hpp"
#include "application/view/promptmenu.hpp"
#include <optional>

MenuController::MenuController() {

}

void MenuController::init(KakouneClient** focused_client, PaneLayout* layout_controller, Window* window, domain::FontManager* font_manager, PromptMenuView* prompt_menu_view, InlineMenuView* inline_menu_view, SearchMenuView *search_menu_view, std::function<void()> mark_dirty) {
    m_focused_client = focused_client;
    m_pane_layout = layout_controller;
    m_window = window;
    m_font_manager = font_manager;
    m_inline_menu_view = inline_menu_view;
    m_prompt_menu_view = prompt_menu_view;
    m_search_menu_view = search_menu_view;
    m_mark_dirty = mark_dirty;
}

void MenuController::render() {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return;
    }

    RenderContext render_context = {
        m_font_manager,
        (*m_focused_client)->state.default_face,
        (*m_focused_client)->uiOptions(),
        static_cast<float>(m_window->getWidth()),
        static_cast<float>(m_window->getHeight())
    };

    int cursor_column = -1;
    if (std::holds_alternative<domain::StatusLinePosition>((*m_focused_client)->state.cursor_position)) {
        cursor_column = std::get<domain::StatusLinePosition>((*m_focused_client)->state.cursor_position).column;
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->render(render_context, (*m_focused_client)->inline_menu_state, *(*m_focused_client)->state.menu, m_pane_layout->findPaneForClient(*m_focused_client)->bounds);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->render(render_context, (*m_focused_client)->prompt_menu_state, *(*m_focused_client)->state.menu, cursor_column);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->render(render_context, (*m_focused_client)->search_menu_state, *(*m_focused_client)->state.menu, cursor_column);
            break;
    }
}

domain::MouseMoveResult MenuController::onMouseMove(float x, float y) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return domain::MouseMoveResult{std::nullopt};
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->onMouseMove(x, y, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->onMouseMove(x, y, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->onMouseMove(x, y, *(*m_focused_client)->state.menu);
    }
    return domain::MouseMoveResult{std::nullopt};
}

float MenuController::x() const {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) return 0.0f;

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->x();
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->x();
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->x();
    }
    return 0;
}

float MenuController::y() const {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) return 0.0f;

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->y();
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->y();
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->y();
    }
    return 0;
}

float MenuController::width() const {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) return 0.0f;

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->width();
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->width();
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->width();
    }
    return 0;
}

float MenuController::height() const {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) return 0.0f;

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->height();
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->height();
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->height();
    }
    return 0;
}

bool MenuController::onMouseButton(domain::MouseButtonEvent event) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu || !(*m_focused_client)->state.menu->hasItems()) {
        return false;
    }

    if (event.button != domain::MouseButton::LEFT || event.action != domain::MouseButtonAction::PRESS) {
        return false;
    }


    std::optional<int> clicked_item_index;

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            clicked_item_index = m_inline_menu_view->findItemAtPosition(event.x, event.y, (*m_focused_client)->inline_menu_state, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::PROMPT:
            clicked_item_index = m_prompt_menu_view->findItemAtPosition(event.x, event.y, (*m_focused_client)->prompt_menu_state, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::SEARCH:
            clicked_item_index = m_search_menu_view->findItemAtPosition(event.x, event.y, (*m_focused_client)->search_menu_state, *(*m_focused_client)->state.menu);
            break;
    }

    if (clicked_item_index.has_value()) {
        (*m_focused_client)->interface->selectMenuItem(clicked_item_index.value());
        return true;
    }

    return false;
}

void MenuController::onMouseScroll(int scroll_amount) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return;
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->onMouseScroll((*m_focused_client)->inline_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->onMouseScroll((*m_focused_client)->prompt_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->onMouseScroll((*m_focused_client)->search_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
    }

    if (m_mark_dirty) {
        m_mark_dirty();
    }
}

void MenuController::ensureItemVisible(int index) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return;
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->ensureItemVisible((*m_focused_client)->inline_menu_state, index);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->ensureItemVisible((*m_focused_client)->prompt_menu_state, index);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->ensureItemVisible((*m_focused_client)->search_menu_state, index);
            break;
    }
}
