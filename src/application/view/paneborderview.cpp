#include "paneborderview.hpp"
#include "application/view/styling.hpp"
#include "application/model/panelayout.hpp"

void PaneBorderView::init(domain::Renderer *renderer)
{
    m_renderer = renderer;
}

void PaneBorderView::render(const std::vector<Pane> &panes, LayoutType layout_type)
{
    if (panes.size() < 2) {
        return;
    }

    if (layout_type == LayoutType::TALL) {
        const auto &main = panes[0].bounds;
        m_renderer->renderRect(default_border_color, main.x + main.width, main.y, PaneLayout::BORDER_WIDTH, main.height);
        for (size_t i = 1; i + 1 < panes.size(); ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x, b.y + b.height, b.width, PaneLayout::BORDER_WIDTH);
        }
    } else {
        const auto &main = panes[0].bounds;
        m_renderer->renderRect(default_border_color, main.x, main.y + main.height, main.width, PaneLayout::BORDER_WIDTH);
        for (size_t i = 1; i + 1 < panes.size(); ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x + b.width, b.y, PaneLayout::BORDER_WIDTH, b.height);
        }
    }
}
