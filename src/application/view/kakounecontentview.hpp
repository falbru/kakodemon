#ifndef KAKOUNECONTENTVIEW_HPP_INCLUDED
#define KAKOUNECONTENTVIEW_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/coord.hpp"
#include "domain/geometry.hpp"
#include "domain/mouse.hpp"
#include "domain/observerlist.hpp"
#include "domain/ports/renderer.hpp"
#include <functional>

class KakouneContentView
{
  public:
    KakouneContentView();

    void init(domain::Renderer *renderer);

    void render(const RenderContext &render_context, const domain::Lines &lines, const domain::Face &default_face,
                const domain::Rectangle &bounds);

    void handleMouseButton(KakouneClient *client, domain::MouseButtonEvent event, domain::Rectangle bounds);
    void handleMouseMove(KakouneClient *client, float x, float y, domain::Rectangle bounds);
    void handleMouseScroll(KakouneClient *client, float x, float y, domain::Rectangle bounds, int amount);

    domain::ObserverId onMouseButton(
        std::function<void(KakouneClient *, domain::MouseButtonEvent, domain::Coord)> callback);
    domain::ObserverId onMouseMove(std::function<void(KakouneClient *, domain::Coord)> callback);
    domain::ObserverId onMouseScroll(std::function<void(KakouneClient *, domain::Coord, int)> callback);
    void removeObserver(domain::ObserverId id);

    float getCellWidth(domain::Font *font) const;
    float getCellHeight(domain::Font *font) const;

    std::pair<float, float> coordToPixels(domain::Font *font, const domain::Coord &coord, float origin_x,
                                          float origin_y) const;
    domain::Coord pixelToCoord(domain::Font *font, float x, float y, float origin_x, float origin_y) const;

  private:
    domain::Renderer *m_renderer;

    domain::ObserverList<KakouneClient *, domain::MouseButtonEvent, domain::Coord> m_mouse_button_observers;
    domain::ObserverList<KakouneClient *, domain::Coord> m_mouse_move_observers;
    domain::ObserverList<KakouneClient *, domain::Coord, int> m_mouse_scroll_observers;
};

#endif
