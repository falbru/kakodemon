#include "kakounecontentview.hpp"
#include "kakounecontentview.hpp"
#include <freetype/freetype.h>
#include <glm/mat4x4.hpp>

KakouneContentView::KakouneContentView() : m_font("/home/falk/.fonts/MonoLisa/ttf/MonoLisa-Regular.ttf", 14)
{
}

void KakouneContentView::init(std::shared_ptr<opengl::Renderer> renderer)
{
    m_renderer = renderer;
}

void KakouneContentView::render(const std::vector<kakoune::Line> &lines, const kakoune::Face& default_face, float x, float y)
{
    m_renderer->renderLines(m_font, lines, default_face, x, y);
}

int KakouneContentView::getCellWidth() {
    return m_font.getCharacter('A').Advance >> 6;
}

int KakouneContentView::getCellHeight() {
    return m_font.getLineHeight();
}
