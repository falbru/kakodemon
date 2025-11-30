#include "kakounecontentview.hpp"
#include "kakounecontentview.hpp"
#include <freetype/freetype.h>
#include <glm/mat4x4.hpp>
#include <memory>

KakouneContentView::KakouneContentView()
{
    m_font = std::make_shared<opengl::Font>("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 17);
}

void KakouneContentView::init(std::shared_ptr<opengl::Renderer> renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(const std::vector<kakoune::Line> &lines, const kakoune::Face& default_face, float x, float y)
{
    m_renderer->renderLines(m_font, lines, default_face, x, y);
}

float KakouneContentView::getCellWidth() const {
    return m_font->getGlyph('A').width();
}

float KakouneContentView::getCellHeight() const {
    return m_font->getLineHeight();
}

std::pair<float, float> KakouneContentView::coordToPixels(const kakoune::Coord& coord) const {
    float x = getCellWidth() * coord.column;
    float y = getCellHeight() * coord.line;
    return std::make_pair(x, y);
}
