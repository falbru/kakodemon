#ifndef APPLICATION_SCENE_HPP_INCLUDED
#define APPLICATION_SCENE_HPP_INCLUDED

#include "application/controller/infoboxcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/infobox.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/multistyledmenu.hpp"
#include "application/view/statusbar.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"

class Window;

class Scene
{
  public:
    Scene();

    void init(ClientManager *client_manager, KakouneClient **focused_client, PaneLayout *pane_layout,
              KakouneContentView *content_view, StatusBarView *status_bar_view, MultiStyledMenuView *multi_styled_menu,
              InfoBoxView *info_box_view, InfoBoxController *info_box_controller, domain::FontManager *font_manager,
              Window *window);

    void render();

    void setScrollSpeed(double speed);
    double getScrollSpeed() const;

  private:
    domain::MouseMoveResult onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);
    void onMouseScroll(double offset);

    bool hitTestMenu(float x, float y) const;
    bool hitTestInfoBox(float x, float y) const;

    KakouneClient **m_focused_client;
    PaneLayout *m_pane_layout;
    KakouneContentView *m_content_view;
    StatusBarView *m_status_bar_view;
    MultiStyledMenuView *m_multi_styled_menu;
    InfoBoxView *m_info_box_view;
    InfoBoxController *m_info_box_controller;
    domain::FontManager *m_font_manager;
    Window *m_window;

    KakouneClient *m_active_mouse_client = nullptr;
    float m_mouse_x = 0.0f;
    float m_mouse_y = 0.0f;
    double m_scroll_accumulator = 0.0;
    double m_scroll_speed = 2.0;
};

#endif
