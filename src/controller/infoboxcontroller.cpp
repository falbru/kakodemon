#include "controller/infoboxcontroller.hpp"
#include "model/uioptions.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(KakouneClient* kakoune_client, EditorController* editor_controller, InfoBoxView* info_box_view) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_info_box_view = info_box_view;
}

void InfoBoxController::update(const UIOptions& ui_options) {
    if (!m_kakoune_client->info_box_visible) return;

    m_info_box_view->render(*m_kakoune_client, ui_options, m_editor_controller->width(), m_editor_controller->height());
}
