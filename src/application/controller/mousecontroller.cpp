#include "application/controller/mousecontroller.hpp"
#include "application/controller/infoboxcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include "domain/uioptions.hpp"
#include "domain/mouse.hpp"

MouseController::MouseController() {

}

void MouseController::init(KakouneClient* kakoune_client, EditorController* editor_controller, MenuController* menu_controller, InfoBoxController* info_box_controller) {
    m_kakoune_client = kakoune_client;
    m_editor_controller = editor_controller;
    m_menu_controller = menu_controller;
    m_info_box_controller = info_box_controller;
}

domain::MouseMoveResult MouseController::onMouseMove(float x, float y, const domain::UIOptions* ui_options) {
    domain::MouseMoveResult mouse_move_result;
    std::optional<domain::Cursor> cursor;

    bool obscured = false;
    if (m_kakoune_client->state.menu.has_value()) {
        float menu_x = m_menu_controller->x();
        float menu_y = m_menu_controller->y();
        float menu_width = m_menu_controller->width();
        float menu_height = m_menu_controller->height();
        if (x >= menu_x && x <= menu_x + menu_width && y >= menu_y && y <= menu_y + menu_height) {
            obscured = true;
        }
    }
    if (!obscured && m_kakoune_client->state.info_box.has_value()) {
        float info_box_x = m_info_box_controller->x();
        float info_box_y = m_info_box_controller->y();
        float info_box_width = m_info_box_controller->width();
        float info_box_height = m_info_box_controller->height();
        if (x >= info_box_x && x <= info_box_x + info_box_width && y >= info_box_y && y <= info_box_y + info_box_height) {
            obscured = true;
        }
    }

    mouse_move_result = m_editor_controller->onMouseMove(x, y, ui_options, obscured);
    cursor = mouse_move_result.cursor;

    mouse_move_result = m_menu_controller->onMouseMove(x, y);
    if (mouse_move_result.cursor != std::nullopt) cursor = mouse_move_result.cursor;

    mouse_move_result.cursor = cursor;

    return mouse_move_result;
}

void MouseController::onMouseButton(domain::MouseButtonEvent event, const domain::UIOptions *ui_options) {
    bool obscured = false;
    if (m_kakoune_client->state.menu.has_value()) {
        float menu_x = m_menu_controller->x();
        float menu_y = m_menu_controller->y();
        float menu_width = m_menu_controller->width();
        float menu_height = m_menu_controller->height();
        if (event.x >= menu_x && event.x <= menu_x + menu_width && event.y >= menu_y && event.y <= menu_y + menu_height) {
            obscured = true;
        }
    }
    if (!obscured && m_kakoune_client->state.info_box.has_value()) {
        float info_box_x = m_info_box_controller->x();
        float info_box_y = m_info_box_controller->y();
        float info_box_width = m_info_box_controller->width();
        float info_box_height = m_info_box_controller->height();
        if (event.x >= info_box_x && event.x <= info_box_x + info_box_width && event.y >= info_box_y && event.y <= info_box_y + info_box_height) {
            obscured = true;
        }
    }

    bool handled_by_menu = m_menu_controller->onMouseButton(event, ui_options);

    m_editor_controller->onMouseButton(event, ui_options, obscured | handled_by_menu);
}

void MouseController::onMouseScroll(double offset, float x, float y, const domain::UIOptions *ui_options) {
    m_scroll_accumulator += offset * m_scroll_speed;

    int scroll_amount = 0;
    if (m_scroll_accumulator >= 1.0) {
        scroll_amount = static_cast<int>(m_scroll_accumulator);
        m_scroll_accumulator -= scroll_amount;
    } else if (m_scroll_accumulator <= -1.0) {
        scroll_amount = static_cast<int>(m_scroll_accumulator);
        m_scroll_accumulator -= scroll_amount;
    }

    if (scroll_amount != 0) {
        bool over_menu = false;
        if (m_kakoune_client->state.menu.has_value()) {
            float menu_x = m_menu_controller->x();
            float menu_y = m_menu_controller->y();
            float menu_width = m_menu_controller->width();
            float menu_height = m_menu_controller->height();
            if (x >= menu_x && x <= menu_x + menu_width && y >= menu_y && y <= menu_y + menu_height) {
                over_menu = true;
            }
        }

        bool over_info_box = false;
        if (!over_menu && m_kakoune_client->state.info_box.has_value()) {
            float info_box_x = m_info_box_controller->x();
            float info_box_y = m_info_box_controller->y();
            float info_box_width = m_info_box_controller->width();
            float info_box_height = m_info_box_controller->height();
            if (x >= info_box_x && x <= info_box_x + info_box_width && y >= info_box_y && y <= info_box_y + info_box_height) {
                over_info_box = true;
            }
        }

        if (over_menu) {
            m_menu_controller->onMouseScroll(-scroll_amount);
        } else if (over_info_box) {
            m_info_box_controller->onMouseScroll(-scroll_amount);
        } else {
            m_editor_controller->onMouseScroll(-scroll_amount, x, y, ui_options);
        }
    }
}

void MouseController::setScrollSpeed(double speed) {
    m_scroll_speed = speed;
}

double MouseController::getScrollSpeed() const {
    return m_scroll_speed;
}
