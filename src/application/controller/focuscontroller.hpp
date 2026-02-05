#ifndef CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED
#define CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "domain/mouse.hpp"

class FocusController
{
  public:
    void init(KakouneClient **focused_client, ClientManager *client_manager, PaneLayout *layout_controller);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);

  private:
    KakouneClient **m_focused_client;
    PaneLayout *m_pane_layout;
    ClientManager *m_client_manager;
    bool m_is_mouse_pressed = false;
};

#endif
