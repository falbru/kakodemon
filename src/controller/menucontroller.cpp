#include "controller/menucontroller.hpp"
#include "kakoune/menustyle.hpp"
#include "view/promptmenu.hpp"

MenuController::MenuController() {

}

void MenuController::init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<EditorController> editor_controller, std::shared_ptr<PromptMenuView> prompt_menu_view, std::shared_ptr<InlineMenuView> inline_menu_view) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_inline_menu_view = inline_menu_view;
    m_prompt_menu_view = prompt_menu_view;
}

void MenuController::update() {
    switch(m_kakoune_client->menu_style) {
        case kakoune::MenuStyle::INLINE:
            if (m_kakoune_client->menu_visible)
                m_inline_menu_view->render(*m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
        case kakoune::MenuStyle::PROMPT:
            m_prompt_menu_view->render(*m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
            break;
    }
}
