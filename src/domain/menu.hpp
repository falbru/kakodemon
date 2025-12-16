#ifndef DOMAIN_MENU_HPP_INCLUDED
#define DOMAIN_MENU_HPP_INCLUDED

#include "domain/coord.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"
#include "domain/statusline.hpp"

namespace domain
{

enum class MenuStyle
{
    PROMPT,
    INLINE,
    SEARCH
};

class Menu

{
  public:
    Menu(std::vector<Line> items, int selected_index, StatusLine input, Coord anchor, Face face, Face selected_face,
         MenuStyle style);

    const Lines &getItems() const;
    const StatusLine &getInput() const;
    const Coord &getAnchor() const;
    const Face &getFace() const;
    const Face &getSelectedFace() const;
    int getSelectedIndex() const;
    MenuStyle getStyle() const;

  private:
    Lines m_items;
    StatusLine m_input;
    Coord m_anchor;
    Face m_face;
    Face m_selected_face;
    int m_selected_index;
    MenuStyle m_style;
};

} // namespace domain

#endif
