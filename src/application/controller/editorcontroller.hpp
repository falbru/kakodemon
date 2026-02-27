#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"
#include <map>

class MenuController;
class Window;

class EditorController
{
  public:
    EditorController();

    void init(ClientManager *client_manager, PaneLayout *pane_layout, KakouneContentView *kakoune_content_view,
              StatusBarView *status_bar_view, domain::FontManager *font_manager, Window *window,
              MenuController *menu_controller);
    void update();

  private:
    void resizeClientsToPaneLayout(const std::vector<Pane> &panes);

    ClientManager *m_client_manager;
    PaneLayout *m_pane_layout;
    KakouneContentView *m_kakoune_content_view;
    StatusBarView *m_status_bar_view;
    domain::FontManager *m_font_manager;
    MenuController *m_menu_controller;
    Window *m_window;

    std::map<domain::MouseButton, bool> m_mouse_button_pressed;
};

#endif
