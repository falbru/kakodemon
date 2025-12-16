#ifndef DOMAIN_MODELINE_HPP_INCLUDED
#define DOMAIN_MODELINE_HPP_INCLUDED

#include "domain/face.hpp"
#include "domain/line.hpp"
#include "domain/statusline.hpp"
#include <optional>

namespace domain
{

class ModeLine
{
  public:
    ModeLine(std::optional<StatusLine> status_line, Line mode_line, Face default_face);

    const std::optional<StatusLine> &getStatusLine() const;
    const Line &getModeLine() const;
    const Face &getDefaultFace() const;

  private:
    std::optional<StatusLine> m_status_line;
    Line m_mode_line;
    Face m_default_face;
};

} // namespace domain

#endif
