#ifndef VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED
#define VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "opengl/font.hpp"
#include "opengl/renderer.hpp"
#include "view/layoutmanager.hpp"
#include "view/widgets/scrollbar.hpp"
#include <memory>
class ScrolledMenuItems
{
  public:
    ScrolledMenuItems(std::shared_ptr<opengl::Font> font, int max_items);

    void render(std::shared_ptr<opengl::Renderer> renderer, const KakouneClient &kakoune_client,
                LayoutManager &layout);

    float width() const;
    float height() const;

  private:
    float m_scroll_offset = 0;
    int m_max_visible_items = 0;
    std::shared_ptr<opengl::Font> m_font;
    std::unique_ptr<ScrollBar> m_scroll_bar;
};

#endif
