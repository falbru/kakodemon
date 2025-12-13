#ifndef CONTROLLER_MENUCONTROLLER_HPP_INCLUDED
#define CONTROLLER_MENUCONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/promptmenu.hpp"
#include "kakoune/menustyle.hpp"

class MenuController
{
  public:
    MenuController();

    void init(KakouneClient *kakoune_client, EditorController *editor_controller, PromptMenuView *prompt_menu_view,
              InlineMenuView *inline_menu_view);

    void update(const UIOptions &ui_options);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    KakouneClient *m_kakoune_client;
    EditorController *m_editor_controller;
    InlineMenuView *m_inline_menu_view;
    PromptMenuView *m_prompt_menu_view;

    kakoune::MenuStyle m_current_style;
};

#endif
