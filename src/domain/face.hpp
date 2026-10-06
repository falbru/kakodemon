#ifndef DOMAIN_FACE_HPP_INCLUDED
#define DOMAIN_FACE_HPP_INCLUDED

#include "domain/color.hpp"
#include "domain/uioptions.hpp"
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

class DefaultFace
{
  public:
    DefaultFace(OptionalColor bg, OptionalColor fg);
    DefaultFace(OptionalColor bg, OptionalColor fg, std::vector<Attribute> attributes);

    RGBAColor resolveBg(const ColorOverrides &color_overrides) const;
    RGBAColor resolveFg(const ColorOverrides &color_overrides) const;
    const std::vector<Attribute> &getAttributes() const;

    bool hasAttribute(Attribute attribute) const;

    bool operator==(const DefaultFace &other) const;

  private:
    OptionalColor m_bg;
    OptionalColor m_fg;
    std::vector<Attribute> m_attributes;
};

class ResolvedFace
{
  public:
    ResolvedFace(RGBAColor bg, RGBAColor fg);
    ResolvedFace(RGBAColor bg, RGBAColor fg, std::vector<Attribute> attributes);

    RGBAColor getBg() const;
    RGBAColor getFg() const;

    const std::vector<Attribute> &getAttributes() const;
    bool hasAttribute(Attribute attribute) const;

    bool operator==(const ResolvedFace &other) const;

  private:
    RGBAColor m_bg;
    RGBAColor m_fg;
    std::vector<Attribute> m_attributes;
};

class Face
{
  public:
    Face(OptionalColor bg, OptionalColor fg);
    Face(OptionalColor bg, OptionalColor fg, std::vector<Attribute> attributes);

    RGBAColor resolveBg(const DefaultFace &default_face, const ColorOverrides &color_overrides) const;
    RGBAColor resolveBg(const Face &fallback_face, const DefaultFace &default_face,
                        const ColorOverrides &color_overrides) const;

    RGBAColor resolveFg(const DefaultFace &default_face, const ColorOverrides &color_overrides) const;
    RGBAColor resolveFg(const Face &fallback_face, const DefaultFace &default_face,
                        const ColorOverrides &color_overrides) const;

    ResolvedFace resolve(const DefaultFace &default_face, const ColorOverrides &color_overrides) const;
    ResolvedFace resolve(const Face &fallback_face, const DefaultFace &default_face,
                         const ColorOverrides &color_overrides) const;

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
