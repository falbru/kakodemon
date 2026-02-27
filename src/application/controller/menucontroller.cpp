#include "menucontroller.hpp"
#include "domain/editor.hpp"
#include "domain/mouse.hpp"
#include "application/view/promptmenu.hpp"
#include <optional>

MenuController::MenuController() {

}

void MenuController::init(KakouneClient** focused_client, PaneLayout* layout_controller, Window* window, domain::FontManager* font_manager, PromptMenuView* prompt_menu_view, InlineMenuView* inline_menu_view, SearchMenuView *search_menu_view) {
    m_focused_client = focused_client;
    m_pane_layout = layout_controller;
    m_window = window;
    m_font_manager = font_manager;
    m_inline_menu_view = inline_menu_view;
    m_prompt_menu_view = prompt_menu_view;
    m_search_menu_view = search_menu_view;

    m_prompt_menu_view->onMouseButton([this](int item_index) {
        (*m_focused_client)->interface->selectMenuItem(item_index);
    });

    m_inline_menu_view->onMouseButton([this](int item_index) {
        (*m_focused_client)->interface->selectMenuItem(item_index);
    });

    m_search_menu_view->onMouseButton([this](int item_index) {
        (*m_focused_client)->interface->selectMenuItem(item_index);
    });
}

domain::MouseMoveResult MenuController::handleMouseMove(float x, float y) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return domain::MouseMoveResult{std::nullopt};
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->handleMouseMove(x, y, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->handleMouseMove(x, y, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->handleMouseMove(x, y, *(*m_focused_client)->state.menu);
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

bool MenuController::handleMouseButton(domain::MouseButtonEvent event) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu || !(*m_focused_client)->state.menu->hasItems()) {
        return false;
    }

    if (event.button != domain::MouseButton::LEFT || event.action != domain::MouseButtonAction::PRESS) {
        return false;
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->handleMouseButton(event, (*m_focused_client)->inline_menu_state, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->handleMouseButton(event, (*m_focused_client)->prompt_menu_state, *(*m_focused_client)->state.menu);
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->handleMouseButton(event, (*m_focused_client)->search_menu_state, *(*m_focused_client)->state.menu);
    }

    return false;
}

void MenuController::handleMouseScroll(int scroll_amount) {
    if (!*m_focused_client || !(*m_focused_client)->state.menu.has_value()) {
        return;
    }

    switch((*m_focused_client)->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->handleMouseScroll((*m_focused_client)->inline_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->handleMouseScroll((*m_focused_client)->prompt_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->handleMouseScroll((*m_focused_client)->search_menu_state, scroll_amount, *(*m_focused_client)->state.menu);
            break;
    }

    m_window->setNeedsRerender();
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
