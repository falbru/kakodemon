#include "domain/menu.hpp"
#include "domain/statusline.hpp"

namespace domain {

Menu::Menu(std::vector<Line> items, int selected_index, StatusLine input, Coord anchor, Face face, Face selected_face, MenuStyle style) : m_items(items), m_selected_index(selected_index), m_input(input), m_anchor(anchor), m_face(face), m_selected_face(selected_face), m_style(style) {

}

const StatusLine& Menu::getInput() const {
    return m_input;
}

const Lines& Menu::getItems() const {
    return m_items;
}

const Coord& Menu::getAnchor() const {
    return m_anchor;
}

const Face& Menu::getFace() const {
    return m_face;
}

const Face& Menu::getSelectedFace() const {
    return m_selected_face;
}

int Menu::getSelectedIndex() const {
    return m_selected_index;
}

MenuStyle Menu::getStyle() const {
    return m_style;

}

}
