#include "kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"
#include "domain/geometry.hpp"

KakouneContentView::KakouneContentView()
{
}

void KakouneContentView::init(domain::Renderer* renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(const RenderContext& render_context, const domain::Lines &lines, const domain::Face& default_face, const domain::Rectangle& bounds)
{
    m_renderer->addBounds(bounds.x, bounds.y, bounds.width, bounds.height);
    m_renderer->renderLines(render_context.textConfig(render_context.ui_options.font_content), lines, default_face, bounds.x, bounds.y);
    m_renderer->popBounds();

    m_width = bounds.width;
    m_height = bounds.height;
}

float KakouneContentView::getCellWidth(domain::Font* font) const {
    return font->getGlyphMetrics('A').advance; // TODO use builtin size instead of hardcoding for the A-letter
}

float KakouneContentView::getCellHeight(domain::Font* font) const {
    return font->getLineHeight();
}

std::pair<float, float> KakouneContentView::coordToPixels(domain::Font* font, const domain::Coord& coord) const {
    float x = getCellWidth(font) * coord.column;
    float y = getCellHeight(font) * coord.line;
    return {x, y};
}

domain::Coord KakouneContentView::pixelToCoord(domain::Font* font, float x, float y) const {
    int column = static_cast<int>(x / getCellWidth(font));
    int line = static_cast<int>(y / getCellHeight(font));
    return {line, column};
}

float KakouneContentView::width() const {
    return m_width;
}

float KakouneContentView::height() const {
    return m_height;
}
