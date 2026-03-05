#include "paneborderview.hpp"
#include "application/view/styling.hpp"
#include "application/model/panelayout.hpp"

void PaneBorderView::init(domain::Renderer *renderer)
{
    m_renderer = renderer;
}

void PaneBorderView::render(const std::vector<Pane> &panes)
{
    for (size_t i = 0; i + 1 < panes.size(); ++i) {
        const auto &bounds = panes[i].bounds;
        m_renderer->renderRect(
            default_border_color,
            bounds.x + bounds.width,
            bounds.y,
            PaneLayout::BORDER_WIDTH,
            bounds.height
        );
    }
}
