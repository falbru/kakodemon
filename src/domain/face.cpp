#include "face.hpp"
#include "domain/color.hpp"
#include <variant>

namespace domain {

Face::Face(OptionalColor bg, OptionalColor fg) : m_bg(bg), m_fg(fg) {

}

RGBAColor Face::getFg(const Face& default_face) const {
    if (std::holds_alternative<DefaultColor>(m_fg)) {
        return default_face.getFg();
    }else if (std::holds_alternative<FixedColor>(m_fg)) {
        return getRGBAColor(std::get<FixedColor>(m_fg));
    }else {
        return std::get<RGBAColor>(m_fg);
    }
}

RGBAColor Face::getFg() const {
    if (std::holds_alternative<DefaultColor>(m_fg)) {
        return getDefaultFg();
    }else if (std::holds_alternative<FixedColor>(m_fg)) {
        return getRGBAColor(std::get<FixedColor>(m_fg));
    }else {
        return std::get<RGBAColor>(m_fg);
    }
}

RGBAColor Face::getBg(const Face& default_face) const {
    if (std::holds_alternative<DefaultColor>(m_bg)) {
        return default_face.getBg();
    }else if (std::holds_alternative<FixedColor>(m_bg)) {
        return getRGBAColor(std::get<FixedColor>(m_bg));
    }else {
        return std::get<RGBAColor>(m_bg);
    }
}

RGBAColor Face::getBg() const {
    if (std::holds_alternative<DefaultColor>(m_bg)) {
        return getDefaultBg();
    }else if (std::holds_alternative<FixedColor>(m_bg)) {
        return getRGBAColor(std::get<FixedColor>(m_bg));
    }else {
        return std::get<RGBAColor>(m_bg);
    }
}

}
