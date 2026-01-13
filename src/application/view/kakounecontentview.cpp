#include "kakounecontentview.hpp"
#include "application/view/rendercontext.hpp"

KakouneContentView::KakouneContentView()
{
}

void KakouneContentView::init(domain::Renderer* renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(const RenderContext& render_context, const domain::Lines &lines, const domain::Face& default_face, float x, float y)
{
    m_renderer->renderLines(render_context.textConfig(render_context.ui_options.font), lines, default_face, x, y);
}

float KakouneContentView::getCellWidth(domain::Font* font) const {
    return font->getGlyphMetrics('A').advance;
}

float KakouneContentView::getCellHeight(domain::Font* font) const {
    return font->getLineHeight();
}

std::pair<float, float> KakouneContentView::coordToPixels(domain::Font* font, const domain::Coord& coord) const {
    float x = getCellWidth(font) * coord.column;
    float y = getCellHeight(font) * coord.line;
    return {x, y};
}

void KakouneContentView::setWidth(float width) {
    m_width = width;
}

void KakouneContentView::setHeight(float height) {
    m_height = height;
}

float KakouneContentView::width() const {
    return m_width;
}

float KakouneContentView::height() const {
    return m_height;
}
