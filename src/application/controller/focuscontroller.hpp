#ifndef CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED
#define CONTROLLER_FOCUSCONTROLLER_HPP_INCLUDED

#include "application/controller/layoutcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/mouse.hpp"

class FocusController
{
  public:
    void init(KakouneClient **focused_client, LayoutController *layout_controller);
    void onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);

  private:
    KakouneClient **m_focused_client;
    LayoutController *m_layout_controller;
    bool m_is_mouse_pressed = false;
};

#endif
