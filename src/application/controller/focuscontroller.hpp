#ifndef CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED
#define CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/window.hpp"

class FocusController
{
  public:
    void init(FocusedClientStack *focused_client_stack, ClientManager *client_manager, PaneLayout *layout_controller,
              domain::Window *window);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);

  private:
    FocusedClientStack *m_focused_client_stack;
    PaneLayout *m_pane_layout;
    ClientManager *m_client_manager;
    bool m_is_mouse_pressed = false;
};

#endif
