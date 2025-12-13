#include "scrollbar.hpp"
#include "domain/color.hpp"

ScrollBar::ScrollBar() {

}

void ScrollBar::setValue(float value, float max_value, float visible_range) {
    m_value = value;
    m_max_value = max_value;
    m_visible_range = visible_range;
}

void ScrollBar::render(Renderer* renderer, domain::Color color, LayoutManager &layout) const {
    float height = (m_visible_range / (m_max_value + m_visible_range)) * layout.current().height;
    float x = layout.current().x + layout.current().width - WIDTH;
    float y = layout.current().y + (m_value / m_max_value) * (layout.current().height - height);

    renderer->renderRect(
        color,
        x,
        y,
        WIDTH, height);
}

float ScrollBar::width() const {
    return WIDTH;
}
