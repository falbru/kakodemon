#ifndef CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED
#define CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED

#include "application/model/clientmanager.hpp"
#include "application/model/focusedclientstack.hpp"
#include "application/model/panelayout.hpp"
#include "application/view/multistyledmenu.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/window.hpp"

class FocusController
{
  public:
    void init(FocusedClientStack *focused_client_stack, ClientManager *client_manager, PaneLayout *layout_controller,
              domain::Window *window, MultiStyledMenuView *menu_view);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);

  private:
    FocusedClientStack *m_focused_client_stack;
    PaneLayout *m_pane_layout;
    ClientManager *m_client_manager;
    MultiStyledMenuView *m_menu_view;
    bool m_is_mouse_pressed = false;
};

#endif
