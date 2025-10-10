#ifndef CONTROLLER_MENUCONTROLLER_HPP_INCLUDED
#define CONTROLLER_MENUCONTROLLER_HPP_INCLUDED

#include "controller/editorcontroller.hpp"
#include "view/inlinemenu.hpp"
#include "view/promptmenu.hpp"

class MenuController
{
  public:
    MenuController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<EditorController> editor_controller,
              std::shared_ptr<PromptMenuView> prompt_menu_view, std::shared_ptr<InlineMenuView> inline_menu_view);

    void update();

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<EditorController> m_editor_controller;
    std::shared_ptr<InlineMenuView> m_inline_menu_view;
    std::shared_ptr<PromptMenuView> m_prompt_menu_view;
};

#endif
