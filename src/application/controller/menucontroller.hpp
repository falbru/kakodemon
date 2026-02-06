#ifndef CONTROLLER_MENUCONTROLLER_HPP_INCLUDED
#define CONTROLLER_MENUCONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/searchmenu.hpp"
#include "application/window.hpp"
#include "domain/mouse.hpp"
#include <functional>

class MenuController
{
  public:
    MenuController();

    void init(KakouneClient **focused_client, PaneLayout *layout_controller, Window *window,
              domain::FontManager *font_manager, PromptMenuView *prompt_menu_view, InlineMenuView *inline_menu_view,
              SearchMenuView *search_menu_view, std::function<void()> mark_dirty);

    void render();

    domain::MouseMoveResult onMouseMove(float x, float y);
    bool onMouseButton(domain::MouseButtonEvent event);
    void onMouseScroll(int scroll_amount);
    void ensureItemVisible(int index);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    KakouneClient **m_focused_client;
    Window *m_window;
    PaneLayout *m_pane_layout;
    domain::FontManager *m_font_manager;
    InlineMenuView *m_inline_menu_view;
    PromptMenuView *m_prompt_menu_view;
    SearchMenuView *m_search_menu_view;

    std::function<void()> m_mark_dirty;
};

#endif
