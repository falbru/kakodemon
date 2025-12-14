#ifndef CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED
#define CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED

#include "application/controller/editorcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/view/infobox.hpp"

class InfoBoxController
{
  public:
    InfoBoxController();

    void init(KakouneClient *kakoune_client, EditorController *editor_controller, InfoBoxView *info_box_view);

    void update(const UIOptions &ui_options);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    KakouneClient *m_kakoune_client;
    EditorController *m_editor_controller;
    InfoBoxView *m_info_box_view;
};

#endif
