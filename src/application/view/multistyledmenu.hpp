#ifndef VIEW_MULTISTYLEDMENU_HPP_INCLUDED
#define VIEW_MULTISTYLEDMENU_HPP_INCLUDED

#include "application/model/viewstate.hpp"
#include "application/observerlist.hpp"
#include "application/view/inlinemenu.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/promptmenu.hpp"
#include "application/view/rendercontext.hpp"
#include "application/view/searchmenu.hpp"
#include "domain/geometry.hpp"
#include "domain/menu.hpp"
#include "domain/mouse.hpp"
#include <functional>
#include <memory>
#include <optional>

class MultiStyledMenuView
{
  public:
    MultiStyledMenuView();

    void init(domain::Renderer *renderer, KakouneContentView *kakoune_content_view);

    void render(const RenderContext &render_context, MultiStyledMenuState &state, const domain::Menu &menu,
                int cursor_column, std::optional<domain::Rectangle> content_bounds);

    domain::MouseMoveResult handleMouseMove(float x, float y, const domain::Menu &menu);
    bool handleMouseButton(domain::MouseButtonEvent event, MultiStyledMenuState &state, const domain::Menu &menu);
    void handleMouseScroll(MultiStyledMenuState &state, int scroll_amount, const domain::Menu &menu);
    void ensureItemVisible(MultiStyledMenuState &state, int index, const domain::Menu &menu);

    ObserverId onMouseButton(std::function<void(int)> callback);
    void removeObserver(ObserverId id);

    float x() const;
    float y() const;
    float width() const;
    float height() const;

  private:
    std::unique_ptr<InlineMenuView> m_inline_menu;
    std::unique_ptr<PromptMenuView> m_prompt_menu;
    std::unique_ptr<SearchMenuView> m_search_menu;

    domain::MenuStyle m_last_style = domain::MenuStyle::PROMPT;

    ObserverList<int> m_mouse_button_observers;

    void forwardMouseButton(int item_index);
};

#endif
