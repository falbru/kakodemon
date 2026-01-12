#ifndef DOMAIN_FACE_HPP_INCLUDED
#define DOMAIN_FACE_HPP_INCLUDED

#include "domain/color.hpp"
#include <unordered_map>
#include <vector>

namespace domain
{

enum class Attribute
{
    Underline,
    CurlyUnderline,
    DoubleUnderline,
    Reverse,
    Blink,
    Bold,
    Dim,
    Italic,
    FinalFg,
    FinalBg,
    FinalAttr
};

class Face
{
  public:
    Face(OptionalColor bg, OptionalColor fg);
    Face(OptionalColor bg, OptionalColor fg, std::vector<Attribute> attributes);

    RGBAColor getFg(const Face &default_face) const;
    RGBAColor getBg(const Face &default_face) const;

    RGBAColor getFg() const;
    RGBAColor getBg() const;

    RGBAColor getFg(const Face &default_face, const std::unordered_map<FixedColor, RGBAColor> &color_overrides) const;
    RGBAColor getBg(const Face &default_face, const std::unordered_map<FixedColor, RGBAColor> &color_overrides) const;

    RGBAColor getFg(const std::unordered_map<FixedColor, RGBAColor> &color_overrides) const;
    RGBAColor getBg(const std::unordered_map<FixedColor, RGBAColor> &color_overrides) const;

    const std::vector<Attribute> &getAttributes() const;
    bool hasAttribute(Attribute attribute) const;

    bool operator==(const Face &other) const;

  private:
    OptionalColor m_bg;
    OptionalColor m_fg;
    std::vector<Attribute> m_attributes;
};

} // namespace domain

#endif
