#include "face.hpp"
#include "domain/color.hpp"
#include <algorithm>
#include <variant>

namespace domain
{

DefaultFace::DefaultFace(OptionalColor bg, OptionalColor fg) : m_bg(bg), m_fg(fg), m_attributes()
{
}

DefaultFace::DefaultFace(OptionalColor bg, OptionalColor fg, std::vector<Attribute> attributes)
    : m_bg(bg), m_fg(fg), m_attributes(std::move(attributes))
{
}

RGBAColor DefaultFace::resolveBg(const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_bg))
    {
        return getDefaultBg();
    }
    else if (std::holds_alternative<NamedColor>(m_bg))
    {
        return getRGBAColor(std::get<NamedColor>(m_bg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_bg);
    }
}

RGBAColor DefaultFace::resolveFg(const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_fg))
    {
        return getDefaultFg();
    }
    else if (std::holds_alternative<NamedColor>(m_fg))
    {
        return getRGBAColor(std::get<NamedColor>(m_fg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_fg);
    }
}

ResolvedFace::ResolvedFace(RGBAColor bg, RGBAColor fg) : m_bg(bg), m_fg(fg), m_attributes()
{
}

ResolvedFace::ResolvedFace(RGBAColor bg, RGBAColor fg, std::vector<Attribute> attributes)
    : m_bg(bg), m_fg(fg), m_attributes(std::move(attributes))
{
}

RGBAColor ResolvedFace::getBg() const
{
    return m_bg;
}

RGBAColor ResolvedFace::getFg() const
{
    return m_fg;
}

const std::vector<Attribute> &DefaultFace::getAttributes() const
{
    return m_attributes;
}

bool DefaultFace::hasAttribute(Attribute attribute) const
{
    return std::find(m_attributes.begin(), m_attributes.end(), attribute) != m_attributes.end();
}

bool DefaultFace::operator==(const DefaultFace &other) const
{
    return m_bg == other.m_bg && m_fg == other.m_fg && m_attributes == other.getAttributes();
}

const std::vector<Attribute> &ResolvedFace::getAttributes() const
{
    return m_attributes;
}

bool ResolvedFace::hasAttribute(Attribute attribute) const
{
    return std::find(m_attributes.begin(), m_attributes.end(), attribute) != m_attributes.end();
}

bool ResolvedFace::operator==(const ResolvedFace &other) const
{
    return m_bg == other.m_bg && m_fg == other.m_fg && m_attributes == other.m_attributes;
}

Face::Face(OptionalColor bg, OptionalColor fg) : m_bg(bg), m_fg(fg), m_attributes()
{
}

Face::Face(OptionalColor bg, OptionalColor fg, std::vector<Attribute> attributes)
    : m_bg(bg), m_fg(fg), m_attributes(std::move(attributes))
{
}

RGBAColor Face::resolveBg(const DefaultFace &default_face, const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_bg))
    {
        return default_face.resolveBg(color_overrides);
    }
    else if (std::holds_alternative<NamedColor>(m_bg))
    {
        return getRGBAColor(std::get<NamedColor>(m_bg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_bg);
    }
}
RGBAColor Face::resolveBg(const Face &fallback_face, const DefaultFace &default_face,
                          const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_bg))
    {
        return fallback_face.resolveBg(default_face, color_overrides);
    }
    else if (std::holds_alternative<NamedColor>(m_bg))
    {
        return getRGBAColor(std::get<NamedColor>(m_bg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_bg);
    }
}

RGBAColor Face::resolveFg(const DefaultFace &default_face, const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_fg))
    {
        return default_face.resolveFg(color_overrides);
    }
    else if (std::holds_alternative<NamedColor>(m_fg))
    {
        return getRGBAColor(std::get<NamedColor>(m_fg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_fg);
    }
}

RGBAColor Face::resolveFg(const Face &fallback_face, const DefaultFace &default_face,
                          const ColorOverrides &color_overrides) const
{
    if (std::holds_alternative<DefaultColor>(m_fg))
    {
        return fallback_face.resolveFg(default_face, color_overrides);
    }
    else if (std::holds_alternative<NamedColor>(m_fg))
    {
        return getRGBAColor(std::get<NamedColor>(m_fg), color_overrides);
    }
    else
    {
        return std::get<RGBAColor>(m_fg);
    }
}

ResolvedFace Face::resolve(const DefaultFace &default_face, const ColorOverrides &color_overrides) const
{
    return ResolvedFace(resolveBg(default_face, color_overrides), resolveFg(default_face, color_overrides),
                        m_attributes);
}

ResolvedFace Face::resolve(const Face &fallback_face, const DefaultFace &default_face,
                           const ColorOverrides &color_overrides) const
{
    return ResolvedFace(resolveBg(fallback_face, default_face, color_overrides),
                        resolveFg(fallback_face, default_face, color_overrides), m_attributes);
}

const std::vector<Attribute> &Face::getAttributes() const
{
    return m_attributes;
}

bool Face::hasAttribute(Attribute attribute) const
{
    return std::find(m_attributes.begin(), m_attributes.end(), attribute) != m_attributes.end();
}

bool Face::operator==(const Face &other) const
{
    return m_bg == other.m_bg && m_fg == other.m_fg && m_attributes == other.m_attributes;
}

} // namespace domain
