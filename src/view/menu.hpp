#ifndef VIEW_MENU_HPP_INCLUDED
#define VIEW_MENU_HPP_INCLUDED

#include "model/kakouneclient.hpp"
#include "opengl/renderer.hpp"
#include <memory>

class MenuView
{
  public:
    MenuView();

    void init(std::shared_ptr<opengl::Renderer> renderer);

    void render(const KakouneClient &kakoune_client, float width, float height);

  private:
    void renderPromptStyle(const KakouneClient &kakoune_client, float width, float height);
    void renderInlineStyle(const KakouneClient &kakoune_client, float width, float height);

    std::shared_ptr<opengl::Renderer> m_renderer;
    opengl::Font m_font;

    int m_scroll_offset;
    const int MAX_PROMPT_ITEMS = 25;
    const int MAX_INLINE_ITEMS = 12;
};

#endif // menu_hpp_INCLUDED
