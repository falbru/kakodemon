#ifndef CONTROLLER_MENUCONTROLLER_HPP_INCLUDED
#define CONTROLLER_MENUCONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/searchmenu.hpp"
#include "domain/mouse.hpp"
#include <functional>

class MenuController
{
  public:
    MenuController();

    void init(KakouneClient *kakoune_client, EditorController *editor_controller, domain::FontManager *font_manager,
              PromptMenuView *prompt_menu_view, InlineMenuView *inline_menu_view, SearchMenuView *search_menu_view,
              std::function<void()> mark_dirty);

    void render(const UIOptions &ui_options);

    domain::MouseMoveResult onMouseMove(float x, float y);
    bool onMouseButton(domain::MouseButtonEvent event, const UIOptions *ui_options);
    void onMouseScroll(int scroll_amount);
    void ensureItemVisible(int index);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    KakouneClient *m_kakoune_client;
    EditorController *m_editor_controller;
    domain::FontManager *m_font_manager;
    InlineMenuView *m_inline_menu_view;
    PromptMenuView *m_prompt_menu_view;
    SearchMenuView *m_search_menu_view;

    std::function<void()> m_mark_dirty;
};

#endif
