#ifndef VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED
#define VIEW_WIDGETS_SCROLLEDMENUITEMS_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/widgets/scrollbar.hpp"
#include "domain/ports/renderer.hpp"
#include <memory>

class ScrolledMenuItems
{
  public:
    ScrolledMenuItems(int max_items);

    void render(domain::Renderer *renderer, domain::Font *font, domain::FontManager *font_manager,
                const KakouneClient &kakoune_client, LayoutManager &layout);

    float width() const;
    float height() const;
    float x() const;
    float y() const;

    std::optional<int> findItemAtPosition(float x, float y, domain::Font *font, const KakouneClient &kakoune_client);

  private:
    float m_scroll_offset = 0;
    int m_max_visible_items = 0;
    std::unique_ptr<ScrollBar> m_scroll_bar;
    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_height = 0;
};

#endif
