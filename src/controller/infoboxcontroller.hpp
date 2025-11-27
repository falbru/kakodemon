#ifndef CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED
#define CONTROLLER_INFOBOXCONTROLLER_HPP_INCLUDED

#include "controller/editorcontroller.hpp"
#include "model/kakouneclient.hpp"
#include "view/infobox.hpp"

class InfoBoxController
{
  public:
    InfoBoxController();

    void init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<EditorController> editor_controller, std::shared_ptr<InfoBoxView> info_box_view);

    void update();

  private:
    std::shared_ptr<KakouneClient> m_kakoune_client;
    std::shared_ptr<EditorController> m_editor_controller;
    std::shared_ptr<InfoBoxView> m_info_box_view;
};

#endif
