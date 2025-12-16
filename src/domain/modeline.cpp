#include "domain/modeline.hpp"

namespace domain {

ModeLine::ModeLine(std::optional<StatusLine> status_line, Line mode_line, Face default_face) : m_status_line(status_line), m_mode_line(mode_line), m_default_face(default_face) {

}

const std::optional<StatusLine>& ModeLine::getStatusLine() const {
    return m_status_line;
}

const Line& ModeLine::getModeLine() const {
    return m_mode_line;
}

const Face& ModeLine::getDefaultFace() const {
    return m_default_face;
}

}
