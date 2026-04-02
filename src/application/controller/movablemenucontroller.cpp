#include "application/controller/movablemenucontroller.hpp"
#include "domain/keys.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/window.hpp"
#include <cmath>
#include <variant>

MovableMenuController::MovableMenuController() : m_is_dragging(false) {
}

MovableMenuController::~MovableMenuController() {
}

void MovableMenuController::init(MultiStyledMenuView* menu, domain::Window* window) {
    m_menu = menu;
    m_window = window;

    m_event_filter_id = window->installEventFilter([this](const domain::WindowEvent& event) -> bool {
        if (!std::holds_alternative<domain::MouseButtonEvent>(event) && !std::holds_alternative<domain::MouseMoveEvent>(event)) {
            return false;
        }

        if (!m_menu->isVisible()) {
            m_is_dragging = false;
            return false;
        }

        if (std::holds_alternative<domain::MouseButtonEvent>(event)) {
            domain::MouseButtonEvent button_event = std::get<domain::MouseButtonEvent>(event);

            if (button_event.button == domain::MouseButton::LEFT) {
                if (button_event.action == domain::MouseButtonAction::PRESS && (button_event.modifiers & domain::KeyModifier::CONTROL) != 0 && m_menu->bounds().contains(button_event.x, button_event.y)) {
                    m_is_dragging = true;
                    m_last_drag_cursor_x = button_event.x;
                    m_last_drag_cursor_y = button_event.y;
                }else if (button_event.action == domain::MouseButtonAction::RELEASE) {
                    m_is_dragging = false;
                }
            }
        }else if (m_is_dragging) {
            domain::MouseMoveEvent move_event = std::get<domain::MouseMoveEvent>(event);

            float diff_x = round(move_event.x - m_last_drag_cursor_x);
            float diff_y = round(move_event.y - m_last_drag_cursor_y);

            m_menu->setX(m_menu->x() + diff_x);
            m_menu->setY(m_menu->y() + diff_y);

            m_last_drag_cursor_x = move_event.x;
            m_last_drag_cursor_y = move_event.y;

            m_window->setCursor(domain::Cursor::CROSSHAIR);

            m_window->setNeedsRerender();
        }

        return m_is_dragging;
    });

    m_visiblity_changed_observer_id = m_menu->onVisibilityChanged([this](bool visible) {
        m_is_dragging = false;
        m_menu->resetPosition();
    });
}
