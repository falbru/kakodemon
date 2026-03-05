#include "paneborderview.hpp"
#include "application/view/styling.hpp"
#include "application/model/panelayout.hpp"
#include <algorithm>

void PaneBorderView::init(domain::Renderer *renderer)
{
    m_renderer = renderer;
}

void PaneBorderView::render(const std::vector<Pane> &panes, LayoutType layout_type, int num_masters)
{
    if (panes.size() < 2) {
        return;
    }

    size_t master_count = std::min(static_cast<size_t>(num_masters), panes.size());
    size_t stack_count = panes.size() - master_count;

    if (layout_type == LayoutType::TALL) {
        if (stack_count > 0) {
            const auto &first = panes[0].bounds;
            const auto &last_master = panes[master_count - 1].bounds;
            float col_height = last_master.y + last_master.height - first.y;
            m_renderer->renderRect(default_border_color, first.x + first.width, first.y, PaneLayout::BORDER_WIDTH, col_height);
        }
        for (size_t i = 0; i + 1 < master_count; ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x, b.y + b.height, b.width, PaneLayout::BORDER_WIDTH);
        }
        for (size_t i = master_count; i + 1 < panes.size(); ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x, b.y + b.height, b.width, PaneLayout::BORDER_WIDTH);
        }
    } else {
        if (stack_count > 0) {
            const auto &first = panes[0].bounds;
            const auto &last_master = panes[master_count - 1].bounds;
            float row_width = last_master.x + last_master.width - first.x;
            m_renderer->renderRect(default_border_color, first.x, first.y + first.height, row_width, PaneLayout::BORDER_WIDTH);
        }
        for (size_t i = 0; i + 1 < master_count; ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x + b.width, b.y, PaneLayout::BORDER_WIDTH, b.height);
        }
        for (size_t i = master_count; i + 1 < panes.size(); ++i) {
            const auto &b = panes[i].bounds;
            m_renderer->renderRect(default_border_color, b.x + b.width, b.y, PaneLayout::BORDER_WIDTH, b.height);
        }
    }
}
