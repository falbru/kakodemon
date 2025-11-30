#include "view/statusbar.hpp"
#include "core/alignment.hpp"
#include <memory>

StatusBarView::StatusBarView() {
    m_font = std::make_shared<opengl::Font>("/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf", 17);

}

void StatusBarView::init(std::shared_ptr<opengl::Renderer> renderer) {
    m_renderer = renderer;

}

void StatusBarView::render(const kakoune::Line &mode, const kakoune::Face &default_face, float width, float height) {
    m_renderer->renderLine(m_font, mode, default_face, width, height, core::Alignment::bottomRight());
}

int StatusBarView::getCellWidth() {
    return m_font->getGlyph('A').width();

}

int StatusBarView::getCellHeight() {
    return m_font->getLineHeight();
}
