#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "domain/color.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"
#include <functional>
#include <map>

class MenuController;

class EditorController // TODO rename to KakouneContentViewController or something?
{
  public:
    EditorController();

    void init(ClientManager *client_manager, KakouneClient **focused_client, PaneLayout *layout_controller,
              KakouneContentView *kakoune_content_view, StatusBarView *status_bar_view,
              domain::FontManager *font_manager, std::function<void(domain::RGBAColor)> set_clear_color,
              MenuController *menu_controller);
    bool update();
    void render();

    void onWindowResize(int width, int height);
    domain::MouseMoveResult onMouseMove(float x, float y, bool obscured);
    void onMouseButton(domain::MouseButtonEvent event, bool obscured);
    void onMouseScroll(int amount, float x, float y);

    int width() const;
    int height() const;

  private:
    void resizeClientsToPaneLayout(const std::vector<Pane> &panes);

    ClientManager *m_client_manager;
    KakouneClient **m_focused_client;
    PaneLayout *m_pane_layout;
    KakouneContentView *m_kakoune_content_view;
    StatusBarView *m_status_bar_view;
    domain::FontManager *m_font_manager;
    MenuController *m_menu_controller;

    std::function<void(domain::RGBAColor)> m_set_clear_color;
    void setClearColor(domain::RGBAColor color);

    std::map<domain::MouseButton, bool> m_mouse_button_pressed;
    KakouneClient *m_active_mouse_client;

    int m_width;
    int m_height;
    int m_content_height;
};

#endif
