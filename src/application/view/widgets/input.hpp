#ifndef VIEW_WIDGETS_INPUT_HPP_INCLUDED
#define VIEW_WIDGETS_INPUT_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/view/layoutmanager.hpp"
#include "application/view/styling.hpp"
#include "domain/ports/renderer.hpp"
#include "kakoune/line.hpp"

class Input
{
  public:
    Input();

    void setPrompt(const kakoune::Line &prompt);
    void setContent(const kakoune::Line &content);

    void render(domain::Renderer *renderer, domain::Font *font, const KakouneClient &kakoune_client,
                LayoutManager &layout);

    float height(domain::Font *font) const;

  private:
    float m_offset_x = 0.0f;

    kakoune::Line m_prompt;
    kakoune::Line m_content;

    const float BORDER = 1.0f;
    const float PADDING = SPACING_MEDIUM;
};

#endif
