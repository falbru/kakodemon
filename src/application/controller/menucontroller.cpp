#include "menucontroller.hpp"
#include "domain/mouse.hpp"
#include "application/view/promptmenu.hpp"
#include <optional>

MenuController::MenuController() {

}

void MenuController::init(KakouneClient* kakoune_client, EditorController* editor_controller, domain::FontManager* font_manager, PromptMenuView* prompt_menu_view, InlineMenuView* inline_menu_view, SearchMenuView *search_menu_view, std::function<void()> mark_dirty) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_font_manager = font_manager;
    m_inline_menu_view = inline_menu_view;
    m_prompt_menu_view = prompt_menu_view;
    m_search_menu_view = search_menu_view;
    m_mark_dirty = mark_dirty;
}

void MenuController::render(const domain::UIOptions& ui_options) {
    if (!m_kakoune_client->state.menu.has_value()) {
        return;
    }

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->render(ui_options.font, m_font_manager, *m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->render(ui_options, m_font_manager, *m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->render(ui_options.font, m_font_manager, *m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
    }
}

domain::MouseMoveResult MenuController::onMouseMove(float x, float y) {
    if (!m_kakoune_client->state.menu.has_value()) {
        return domain::MouseMoveResult{std::nullopt};
    }

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->onMouseMove(x, y, *m_kakoune_client);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->onMouseMove(x, y, *m_kakoune_client);
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->onMouseMove(x, y, *m_kakoune_client);
    }
    return domain::MouseMoveResult{std::nullopt};
}

float MenuController::x() const {
    if (!m_kakoune_client->state.menu.has_value()) return 0.0f;

    switch(m_kakoune_client->state.menu->getStyle()) {
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
    if (!m_kakoune_client->state.menu.has_value()) return 0.0f;

    switch(m_kakoune_client->state.menu->getStyle()) {
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
    if (!m_kakoune_client->state.menu.has_value()) return 0.0f;

    switch(m_kakoune_client->state.menu->getStyle()) {
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
    if (!m_kakoune_client->state.menu.has_value()) return 0.0f;

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu_view->height();
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu_view->height();
        case domain::MenuStyle::SEARCH:
            return m_search_menu_view->height();
    }
    return 0;
}

bool MenuController::onMouseButton(domain::MouseButtonEvent event, const domain::UIOptions *ui_options) {
    if (!m_kakoune_client->state.menu || !m_kakoune_client->state.menu->hasItems()) {
        return false;
    }

    if (event.button != domain::MouseButton::LEFT || event.action != domain::MouseButtonAction::PRESS) {
        return false;
    }


    std::optional<int> clicked_item_index;

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            clicked_item_index = m_inline_menu_view->findItemAtPosition(event.x, event.y, ui_options->font, *m_kakoune_client);
            break;
        case domain::MenuStyle::PROMPT:
            clicked_item_index = m_prompt_menu_view->findItemAtPosition(event.x, event.y, ui_options->font, *m_kakoune_client);
            break;
        case domain::MenuStyle::SEARCH:
            clicked_item_index = m_search_menu_view->findItemAtPosition(event.x, event.y, ui_options->font, *m_kakoune_client);
            break;
    }

    if (clicked_item_index.has_value()) {
        m_kakoune_client->interface->selectMenuItem(clicked_item_index.value());
        return true;
    }

    return false;
}

void MenuController::onMouseScroll(int scroll_amount) {
    if (!m_kakoune_client->state.menu.has_value()) {
        return;
    }

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->onMouseScroll(scroll_amount, *m_kakoune_client);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->onMouseScroll(scroll_amount, *m_kakoune_client);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->onMouseScroll(scroll_amount, *m_kakoune_client);
            break;
    }

    if (m_mark_dirty) {
        m_mark_dirty();
    }
}

void MenuController::ensureItemVisible(int index) {
    if (!m_kakoune_client->state.menu.has_value()) {
        return;
    }

    switch(m_kakoune_client->state.menu->getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu_view->ensureItemVisible(index);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu_view->ensureItemVisible(index);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu_view->ensureItemVisible(index);
            break;
    }
}
