#include "kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/faceresolver.hpp"
#include "domain/geometry.hpp"
#include "domain/glyphresolver.hpp"
#include "domain/renderlines.hpp"
#include "domain/uioptions.hpp"
#include <cmath>
#include <optional>

KakouneContentView::KakouneContentView()
{
}

void KakouneContentView::init(domain::Renderer *renderer, domain::Window *window)
{
    m_renderer = renderer;
    m_window = window;
}

void KakouneContentView::render(const RenderContext &render_context, const domain::Lines &lines,
                                const domain::Rectangle &bounds)
{
    domain::GlyphResolver glyph_resolver(render_context.ui_options.font_content, render_context.font_manager);
    domain::FaceResolver face_resolver(std::nullopt, render_context.default_face,
                                       render_context.ui_options.color_overrides);

    auto render_lines =
        domain::RenderLines(lines, glyph_resolver, face_resolver, getCellHeight(render_context.ui_options));

    m_renderer->addBounds(bounds.left(), bounds.top(), bounds.width(), bounds.height());
    m_renderer->renderLines(render_context.textConfig(render_context.ui_options.font_content), render_lines,
                            bounds.left(), bounds.top());
    m_renderer->popBounds();
}

void KakouneContentView::handleMouseButton(KakouneClient *client, domain::MouseButtonEvent event,
                                           domain::Rectangle bounds)
{
    domain::Coord coord = pixelToCoord(client->uiOptions(), event.x, event.y, bounds.left(), bounds.top());
    m_mouse_button_observers.notify(client, event, coord);
}

void KakouneContentView::handleMouseMove(KakouneClient *client, float x, float y, domain::Rectangle bounds)
{
    domain::Coord coord = pixelToCoord(client->uiOptions(), x, y, bounds.left(), bounds.top());
    m_mouse_move_observers.notify(client, coord);
}

void KakouneContentView::handleMouseScroll(KakouneClient *client, float x, float y, domain::Rectangle bounds,
                                           int amount)
{
    domain::Coord coord = pixelToCoord(client->uiOptions(), x, y, bounds.left(), bounds.top());
    m_mouse_scroll_observers.notify(client, coord, amount);
}

domain::ObserverId KakouneContentView::onMouseButton(
    std::function<void(KakouneClient *, domain::MouseButtonEvent, domain::Coord)> callback)
{
    return m_mouse_button_observers.addObserver(std::move(callback));
}

domain::ObserverId KakouneContentView::onMouseMove(std::function<void(KakouneClient *, domain::Coord)> callback)
{
    return m_mouse_move_observers.addObserver(std::move(callback));
}

domain::ObserverId KakouneContentView::onMouseScroll(std::function<void(KakouneClient *, domain::Coord, int)> callback)
{
    return m_mouse_scroll_observers.addObserver(std::move(callback));
}

void KakouneContentView::removeObserver(domain::ObserverId id)
{
    m_mouse_button_observers.removeObserver(id);
    m_mouse_move_observers.removeObserver(id);
    m_mouse_scroll_observers.removeObserver(id);
}

float KakouneContentView::getCellWidth(const domain::UIOptions &ui_options) const
{
    return ui_options.font_content->getGlyphMetrics('A').advance;
}

float KakouneContentView::getCellHeight(const domain::UIOptions &ui_options) const
{
    return std::floor(ui_options.font_content->getLineHeight() * (ui_options.line_height_scale / 100.0f));
}

std::pair<float, float> KakouneContentView::coordToPixels(const domain::UIOptions &ui_options,
                                                          const domain::Coord &coord, float origin_x,
                                                          float origin_y) const
{
    float cell_width = getCellWidth(ui_options);
    float cell_height = getCellHeight(ui_options);
    float x = origin_x + cell_width * coord.column;
    float y = origin_y + cell_height * coord.line;
    return {x, y};
}

domain::Coord KakouneContentView::pixelToCoord(const domain::UIOptions &ui_options, float x, float y, float origin_x,
                                               float origin_y) const
{
    float cell_width = getCellWidth(ui_options);
    float cell_height = getCellHeight(ui_options);
    int column = static_cast<int>((x - origin_x) / cell_width);
    int line = static_cast<int>((y - origin_y) / cell_height);
    return {line, column};
}
