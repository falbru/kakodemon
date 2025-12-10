#ifndef CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED
#define CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED

#include "controller/editorcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "view/infobox.hpp"

class InfoBoxController
{
  public:
    InfoBoxController();

    void init(KakouneClient *kakoune_client, EditorController *editor_controller, InfoBoxView *info_box_view);

    void update(const UIOptions &ui_options);

  private:
    KakouneClient *m_kakoune_client;
    EditorController *m_editor_controller;
    InfoBoxView *m_info_box_view;
};

#endif
