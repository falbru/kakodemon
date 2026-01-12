#ifndef STATUSBAR_HPP_INCLUDED
#define STATUSBAR_HPP_INCLUDED

#include "domain/color.hpp"
#include "domain/modeline.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/renderer.hpp"
#include "domain/uioptions.hpp"
#include <unordered_map>

class StatusBarView
{
  public:
    StatusBarView();

    void init(domain::Renderer *renderer);

    void render(const domain::UIOptions &ui_options, domain::FontManager *font_manager,
                const domain::ModeLine &mode_line, float width, float height);

    float height(domain::Font *font);

  private:
    domain::Renderer *m_renderer;
};

#endif // statusbar_hpp_INCLUDED
