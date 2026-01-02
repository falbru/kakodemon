#ifndef DOMAIN_MENU_HPP_INCLUDED
#define DOMAIN_MENU_HPP_INCLUDED

#include "domain/coord.hpp"
#include "domain/line.hpp"
#include "domain/lines.hpp"
#include "domain/statusline.hpp"
#include <optional>

namespace domain
{

enum class MenuStyle
{
    PROMPT,
    INLINE,
    SEARCH
};

struct MenuItems
{
    Lines items;
    Coord anchor;
    Face face;
    Face selected_face;
    int selected_index;

    MenuItems(Lines items, Coord anchor, Face face, Face selected_face, int selected_index)
        : items(std::move(items)), anchor(anchor), face(face), selected_face(selected_face),
          selected_index(selected_index)
    {
    }
};

class Menu

{
  public:
    Menu(StatusLine input, MenuItems items, MenuStyle style);
    Menu(StatusLine input, MenuStyle style);

    const StatusLine &getInput() const;
    bool hasItems() const;
    const MenuItems &getItems() const;
    MenuStyle getStyle() const;

  private:
    StatusLine m_input;
    std::optional<MenuItems> m_items;
    MenuStyle m_style;
};

} // namespace domain

#endif
