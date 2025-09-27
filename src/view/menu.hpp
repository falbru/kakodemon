#ifndef VIEW_MENU_HPP_INCLUDED
#define VIEW_MENU_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include "view/kakounecontentview.hpp"
#include "view/widgets/input.hpp"
#include "view/widgets/scrollbar.hpp"
#include <memory>

class MenuView
{
  public:
    MenuView();

    void init(std::shared_ptr<opengl::Renderer> renderer, std::shared_ptr<KakouneContentView> kakoune_content_view);

    void render(const KakouneClient &kakoune_client, float width, float height);

  private:
    void renderPromptStyle(const KakouneClient &kakoune_client, float width, float height);
    void renderInlineStyle(const KakouneClient &kakoune_client, float width, float height);

    void renderScrolledContent(const KakouneClient &kakoune_client, LayoutManager &layout, int max_items);

    std::shared_ptr<KakouneContentView> m_kakoune_content_view;
    std::shared_ptr<opengl::Renderer> m_renderer;
    std::shared_ptr<opengl::Font> m_font;

    std::unique_ptr<Input> m_input;
    std::unique_ptr<ScrollBar> m_scroll_bar;

    int m_scroll_offset;
    bool m_orientation = false;
    const int MAX_PROMPT_ITEMS = 25;
    const int MAX_INLINE_ITEMS = 12;

    const float PROMPT_WIDTH = 600.0f;
    const float PROMPT_Y_POSITION = 60.0f;
};

#endif // menu_hpp_INCLUDED
