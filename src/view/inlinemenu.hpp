#ifndef VIEW_INLINEMENU_HPP_INCLUDED
#define VIEW_INLINEMENU_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"
#include "view/widgets/scrollbar.hpp"

class InlineMenuView
{
  public:
    InlineMenuView();

    void init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<KakouneContentView> kakoune_content_view);

    void render(const KakouneClient &kakoune_client, float width, float height);

  private:
    int m_scroll_offset;

    const float MAX_MENU_WIDTH = 700;
    const int MAX_VISIBLE_ITEMS = 12;

    void renderScrolledContent(const KakouneClient &kakoune_client, LayoutManager &layout, int max_items);

    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<opengl::Font> m_font;

    std::unique_ptr<ScrollBar> m_scroll_bar;
};

#endif
