#include "kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/geometry.hpp"

KakouneContentView::KakouneContentView()
{
}

void KakouneContentView::init(domain::Renderer *renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(const RenderContext &render_context, const domain::Lines &lines,
                                 const domain::Face &default_face, const domain::Rectangle &bounds)
{
    m_renderer->addBounds(bounds.x, bounds.y, bounds.width, bounds.height);
    m_renderer->renderLines(render_context.textConfig(render_context.ui_options.font_content), lines, default_face,
                             bounds.x, bounds.y);
    m_renderer->popBounds();
}

void KakouneContentView::handleMouseButton(KakouneClient *client, domain::MouseButtonEvent event,
                                        domain::Rectangle bounds)
{
    domain::Coord coord = pixelToCoord(client->uiOptions().font_content, event.x, event.y, bounds.x, bounds.y);
    m_mouse_button_observers.notify(client, event, coord);
}

void KakouneContentView::handleMouseMove(KakouneClient *client, float x, float y, domain::Rectangle bounds)
{
    domain::Coord coord = pixelToCoord(client->uiOptions().font_content, x, y, bounds.x, bounds.y);
    m_mouse_move_observers.notify(client, coord);
}

void KakouneContentView::handleMouseScroll(KakouneClient *client, float x, float y, domain::Rectangle bounds,
                                        int amount)
{
    domain::Coord coord = pixelToCoord(client->uiOptions().font_content, x, y, bounds.x, bounds.y);
    m_mouse_scroll_observers.notify(client, coord, amount);
}

domain::ObserverId KakouneContentView::onMouseButton(
    std::function<void(KakouneClient *, domain::MouseButtonEvent, domain::Coord)> callback)
{
    return m_mouse_button_observers.addObserver(std::move(callback));
}

domain::ObserverId KakouneContentView::onMouseMove(
    std::function<void(KakouneClient *, domain::Coord)> callback)
{
    return m_mouse_move_observers.addObserver(std::move(callback));
}

domain::ObserverId KakouneContentView::onMouseScroll(
    std::function<void(KakouneClient *, domain::Coord, int)> callback)
{
    return m_mouse_scroll_observers.addObserver(std::move(callback));
}

void KakouneContentView::removeObserver(domain::ObserverId id)
{
    m_mouse_button_observers.removeObserver(id);
    m_mouse_move_observers.removeObserver(id);
    m_mouse_scroll_observers.removeObserver(id);
}

float KakouneContentView::getCellWidth(domain::Font *font) const
{
    return font->getGlyphMetrics('A').advance;
}

float KakouneContentView::getCellHeight(domain::Font *font) const
{
    return font->getLineHeight();
}

std::pair<float, float> KakouneContentView::coordToPixels(domain::Font *font, const domain::Coord &coord,
                                                            float origin_x, float origin_y) const
{
    float x = origin_x + getCellWidth(font) * coord.column;
    float y = origin_y + getCellHeight(font) * coord.line;
    return {x, y};
}

domain::Coord KakouneContentView::pixelToCoord(domain::Font *font, float x, float y, float origin_x,
                                                float origin_y) const
{
    int column = static_cast<int>((x - origin_x) / getCellWidth(font));
    int line = static_cast<int>((y - origin_y) / getCellHeight(font));
    return {line, column};
}
