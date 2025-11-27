#include "controller/infoboxcontroller.hpp"

InfoBoxController::InfoBoxController() {

}

void InfoBoxController::init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<EditorController> editor_controller, std::shared_ptr<InfoBoxView> info_box_view) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_info_box_view = info_box_view;
}

void InfoBoxController::update() {
    if (!m_kakoune_client->info_box_visible) return;

    m_info_box_view->render(*m_kakoune_client, m_editor_controller->width(), m_editor_controller->height());
}
