#ifndef CONTROLLER_MOUSECONTROLLER_HPP_INCLUDED
#define CONTROLLER_MOUSECONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/mouse.hpp"

class Window;

class MouseController
{
  public:
    MouseController();

    void init(KakouneClient **focused_client, EditorController *editor_controller, MenuController *menu_controller,
              InfoBoxController *info_box_controller, Window *window, std::function<void()> mark_dirty);

    domain::MouseMoveResult onMouseMove(float x, float y);
    void onMouseButton(domain::MouseButtonEvent event);
    void onMouseScroll(double offset, float x, float y);

    void setScrollSpeed(double speed);
    double getScrollSpeed() const;

  private:
    KakouneClient **m_focused_client;
    EditorController *m_editor_controller;
    MenuController *m_menu_controller;
    InfoBoxController *m_info_box_controller;
    Window *m_window;

    float m_mouse_x = 0.0f;
    float m_mouse_y = 0.0f;

    double m_scroll_accumulator = 0.0;
    double m_scroll_speed = 2.0;
};

#endif
