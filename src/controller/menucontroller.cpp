#include "controller/menucontroller.hpp"
#include "kakoune/menustyle.hpp"
#include "view/promptmenu.hpp"

MenuController::MenuController() {

}

void MenuController::init(KakouneClient* kakoune_client, EditorController* editor_controller, PromptMenuView* prompt_menu_view, InlineMenuView* inline_menu_view) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_inline_menu_view = inline_menu_view;
    m_prompt_menu_view = prompt_menu_view;
}

void MenuController::update(const UIOptions& ui_options) {
    m_current_style = m_kakoune_client->menu_style;

    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            if (m_kakoune_client->menu_visible)
                m_inline_menu_view->render(ui_options.font.get(), *m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
        case kakoune::MenuStyle::PROMPT:
            m_prompt_menu_view->render(ui_options.font.get(), *m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
    }
}

float MenuController::x() const {
    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            return m_inline_menu_view->x();
        case kakoune::MenuStyle::PROMPT:
            return m_prompt_menu_view->x();
    }
    return 0;
}

float MenuController::y() const {
    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            return m_inline_menu_view->y();
        case kakoune::MenuStyle::PROMPT:
            return m_prompt_menu_view->y();
    }
    return 0;
}

float MenuController::width() const {
    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            return m_inline_menu_view->width();
        case kakoune::MenuStyle::PROMPT:
            return m_prompt_menu_view->width();
    }
    return 0;
}

float MenuController::height() const {
    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            return m_inline_menu_view->height();
        case kakoune::MenuStyle::PROMPT:
            return m_prompt_menu_view->height();
    }
    return 0;
}
