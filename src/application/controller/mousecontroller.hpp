#ifndef CONTROLLER_MOUSECONTROLLER_HPP_INCLUDED
#define CONTROLLER_MOUSECONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/uioptions.hpp"
#include "domain/mouse.hpp"

class MouseController
{
  public:
    MouseController();

    void init(KakouneClient *kakoune_client, EditorController *editor_controller, MenuController *menu_controller,
              InfoBoxController *info_box_controller);

    domain::MouseMoveResult onMouseMove(float x, float y, const UIOptions *ui_options);
    void onMouseButton(domain::MouseButtonEvent event, const UIOptions *ui_options);

  private:
    KakouneClient *m_kakoune_client;
    EditorController *m_editor_controller;
    MenuController *m_menu_controller;
    InfoBoxController *m_info_box_controller;
};

#endif
