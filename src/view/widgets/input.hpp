#ifndef VIEW_WIDGETS_INPUT_HPP_INCLUDED
#define VIEW_WIDGETS_INPUT_HPP_INCLUDED

#include "kakoune/line.hpp"
#include "model/kakouneclient.hpp"
#include "renderer.hpp"
#include "view/layoutmanager.hpp"
#include "view/styling.hpp"

class Input
{
  public:
    Input(Font *font);

    void setPrompt(const kakoune::Line &prompt);
    void setContent(const kakoune::Line &content);

    void render(Renderer *renderer, const KakouneClient &kakoune_client, LayoutManager &layout);

    float width() const;
    float height() const;

  private:
    float m_offset_x = 0.0f;

    kakoune::Line m_prompt;
    kakoune::Line m_content;
    Font *m_font;

    const float BORDER = 1.0f;
    const float PADDING = SPACING_MEDIUM;
};

#endif
