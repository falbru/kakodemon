#include "domain/menu.hpp"
#include "domain/statusline.hpp"

namespace domain {

Menu::Menu(StatusLine input, Face input_face, MenuItems items, MenuStyle style)
    : m_input(std::move(input)), m_input_face(input_face), m_items(std::move(items)), m_style(style) {
}

Menu::Menu(StatusLine input, Face input_face, MenuStyle style)
    : m_input(std::move(input)), m_input_face(input_face), m_items(std::nullopt), m_style(style) {
}

const StatusLine& Menu::getInput() const {
    return m_input;
}

const Face &Menu::getInputFace() const {
    return m_input_face;
}

bool Menu::hasItems() const {
    return m_items.has_value();
}

const MenuItems& Menu::getItems() const {
    return m_items.value();
}

MenuStyle Menu::getStyle() const {
    return m_style;
}

}
