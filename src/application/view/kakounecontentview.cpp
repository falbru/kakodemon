#include "kakounecontentview.hpp"
#include "kakounecontentview.hpp"

KakouneContentView::KakouneContentView()
{
}

void KakouneContentView::init(Renderer* renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(Font* font, const std::vector<kakoune::Line> &lines, const kakoune::Face& default_face, float x, float y)
{
    m_renderer->renderLines(font, lines, default_face, x, y);
}

float KakouneContentView::getCellWidth(Font* font) const {
    return font->getGlyphMetrics('A').width();
}

float KakouneContentView::getCellHeight(Font* font) const {
    return font->getLineHeight();
}

std::pair<float, float> KakouneContentView::coordToPixels(Font* font, const kakoune::Coord& coord) const {
    float x = getCellWidth(font) * coord.column;
    float y = getCellHeight(font) * coord.line;
    return std::make_pair(x, y);
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
