#include "editorcontroller.hpp"
#include "application/controller/menucontroller.hpp"
#include "application/model/clientmanager.hpp"
#include "application/window.hpp"
#include "domain/mouse.hpp"
#include "domain/uioptions.hpp"

EditorController::EditorController()
{
}

void EditorController::init(ClientManager *client_manager, PaneLayout *pane_layout,
                             KakouneContentView *kakoune_content_view, StatusBarView *status_bar_view,
                             domain::FontManager *font_manager, Window *window, MenuController *menu_controller)
{
    m_client_manager = client_manager;
    m_pane_layout = pane_layout;
    m_kakoune_content_view = kakoune_content_view;
    m_status_bar_view = status_bar_view;
    m_font_manager = font_manager;
    m_window = window;
    m_menu_controller = menu_controller;

    m_pane_layout->onArrange([this](const std::vector<Pane> &panes) {
        resizeClientsToPaneLayout(panes);
    });

    m_kakoune_content_view->onMouseButton(
        [this](KakouneClient *client, domain::MouseButtonEvent event, domain::Coord coord) {
            if (event.action == domain::MouseButtonAction::PRESS) {
                m_mouse_button_pressed[event.button] = true;
                client->interface->pressMouseButton(event.button, coord.line, coord.column);
            } else {
                m_mouse_button_pressed[event.button] = false;
                client->interface->releaseMouseButton(event.button, coord.line, coord.column);
            }
        });

    m_kakoune_content_view->onMouseMove([this](KakouneClient *client, domain::Coord coord) {
        for (const auto &[button, pressed] : m_mouse_button_pressed) {
            if (pressed) {
                client->interface->moveMouse(coord.line, coord.column);
                return;
            }
        }
    });

    m_kakoune_content_view->onMouseScroll([](KakouneClient *client, domain::Coord coord, int amount) {
        client->interface->scroll(amount, coord.line, coord.column);
    });
}

void EditorController::update()
{
    for (auto &client : m_client_manager->clients()) {
        auto result = client->interface->getNextKakouneStateAndEvents();
        if (result.has_value()) {
            m_window->setNeedsRerender();

            client->state = result->first;
            m_window->setClearColor(client->state.default_face.getBg());

            domain::FrameEvents events = result->second;

            if (events.ui_options_updated) {
                client->setUIOptions(client->interface->getUIOptions(m_font_manager));
            }

            if (events.menu_select && client->state.menu.has_value() && client->state.menu->hasItems()) {
                m_menu_controller->ensureItemVisible(client->state.menu->getItems().selected_index);
            }
        }
    }
}

void EditorController::resizeClientsToPaneLayout(const std::vector<Pane> &panes)
{
    for (const auto &pane : m_pane_layout->getPanes()) {
        float cell_width = m_kakoune_content_view->getCellWidth(pane.client->uiOptions().font_content);
        float cell_height = m_kakoune_content_view->getCellHeight(pane.client->uiOptions().font_content);
        float status_bar_height = m_status_bar_view->height(pane.client->uiOptions().font_statusbar);

        int rows = static_cast<int>((pane.bounds.height - status_bar_height) / cell_height);
        int columns = static_cast<int>(pane.bounds.width / cell_width);
        pane.client->interface->resize(rows, columns);
    }
}
