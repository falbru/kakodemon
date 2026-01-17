#ifndef VIEW_INFOBOX_HPP_INCLUDED
#define VIEW_INFOBOX_HPP_INCLUDED
#include "application/controller/menucontroller.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/statusbar.hpp"
#include "application/view/widgets/scrollbar.hpp"
#include "domain/editor.hpp"
#include "domain/geometry.hpp"
#include "domain/infobox.hpp"
#include "domain/lines.hpp"
#include <memory>

enum class PlacementDirection
{
    ABOVE,
    BELOW,
    LEFT,
    RIGHT,
    CENTER,
    LEFT_WRAPPED,
    RIGHT_WRAPPED,
    FULL
};

enum class CrossAxisAlignment
{
    START,
    CENTER,
    END
};

struct Placement
{
    domain::Lines content;
    domain::Rectangle bounds;
};

class InfoBoxView
{
  public:
    InfoBoxView();
    void init(domain::Renderer *renderer, MenuController *menu_controller, KakouneContentView *kakoune_content_view,
              StatusBarView *status_bar_view);

    std::pair<float, float> calculateInfoBoxPosition(const domain::Rectangle &anchor, float info_box_width,
                                                     float info_box_height, float viewport_width, float viewport_height,
                                                     PlacementDirection direction, CrossAxisAlignment alignment) const;

    std::optional<Placement> tryPlaceInfoBox(PlacementDirection direction, CrossAxisAlignment alignment,
                                             const domain::Lines &content, const domain::Line &title,
                                             const domain::Rectangle &anchor, float layout_width, float layout_height,
                                             domain::Font *font, domain::FontManager *font_manager,
                                             const domain::Rectangle &menu_rectangle,
                                             const domain::CursorPosition &cursor_position, domain::Font *font_content);

    void render(const RenderContext &render_context, const domain::InfoBox &info_box,
                const domain::CursorPosition &cursor_position);

    void onMouseScroll(int scroll_amount);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    const float MIN_WIDTH = 150.0f;
    const float MAX_WIDTH = 1000.0f;
    const float MAX_HEIGHT = 600.0f;
    domain::Renderer *m_renderer;
    KakouneContentView *m_kakoune_content_view;
    MenuController *m_menu_controller;
    StatusBarView *m_status_bar_view;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    float m_scroll_offset = 0.0f;
    std::unique_ptr<ScrollBar> m_scroll_bar;
};

#endif // VIEW_INFOBOX_HPP_INCLUDED
