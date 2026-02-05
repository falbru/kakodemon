#include "layoutcontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/model/kakouneclient.hpp"
#include "application/model/panelayout.hpp"
#include "domain/geometry.hpp"

LayoutController::LayoutController() {

}

void LayoutController::init(PaneLayout* pane_layout, ClientManager* client_manager) {
    m_pane_layout = pane_layout;

    client_manager->onClientAdded([=](KakouneClient*) {
        m_pane_layout->arrange();
    });

    client_manager->onClientRemoved([=](KakouneClient*) {
        m_pane_layout->arrange();
    });
}

void LayoutController::onWindowResize(int width, int height) {
    m_pane_layout->setBounds(domain::Rectangle{0, 0, (float)width, (float)height});
    m_pane_layout->arrange();
}
