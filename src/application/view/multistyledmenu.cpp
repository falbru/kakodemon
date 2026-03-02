#include "multistyledmenu.hpp"

MultiStyledMenuView::MultiStyledMenuView() {}

void MultiStyledMenuView::init(domain::Renderer *renderer, KakouneContentView *kakoune_content_view)
{
    m_inline_menu = std::make_unique<InlineMenuView>();
    m_prompt_menu = std::make_unique<PromptMenuView>();
    m_search_menu = std::make_unique<SearchMenuView>();

    m_inline_menu->init(renderer, kakoune_content_view);
    m_prompt_menu->init(renderer, kakoune_content_view);
    m_search_menu->init(renderer);

    m_inline_menu->onMouseButton([this](int item_index) { forwardMouseButton(item_index); });
    m_prompt_menu->onMouseButton([this](int item_index) { forwardMouseButton(item_index); });
    m_search_menu->onMouseButton([this](int item_index) { forwardMouseButton(item_index); });
}

void MultiStyledMenuView::forwardMouseButton(int item_index)
{
    for (auto &[id, callback] : m_mouse_button_observers) {
        callback(item_index);
    }
}

void MultiStyledMenuView::render(const RenderContext &render_context, MultiStyledMenuState &state,
                                  const domain::Menu &menu, int cursor_column,
                                  std::optional<domain::Rectangle> content_bounds)
{
    m_last_style = menu.getStyle();
    switch (menu.getStyle()) {
        case domain::MenuStyle::INLINE:
            if (content_bounds.has_value()) {
                m_inline_menu->render(render_context, state.inline_state, menu, content_bounds.value());
            }
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu->render(render_context, state.prompt_state, menu, cursor_column);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu->render(render_context, state.search_state, menu, cursor_column);
            break;
    }
}

domain::MouseMoveResult MultiStyledMenuView::handleMouseMove(float x, float y, const domain::Menu &menu)
{
    switch (menu.getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu->handleMouseMove(x, y, menu);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu->handleMouseMove(x, y, menu);
        case domain::MenuStyle::SEARCH:
            return m_search_menu->handleMouseMove(x, y, menu);
    }
    return domain::MouseMoveResult{std::nullopt};
}

bool MultiStyledMenuView::handleMouseButton(domain::MouseButtonEvent event, MultiStyledMenuState &state,
                                             const domain::Menu &menu)
{
    switch (menu.getStyle()) {
        case domain::MenuStyle::INLINE:
            return m_inline_menu->handleMouseButton(event, state.inline_state, menu);
        case domain::MenuStyle::PROMPT:
            return m_prompt_menu->handleMouseButton(event, state.prompt_state, menu);
        case domain::MenuStyle::SEARCH:
            return m_search_menu->handleMouseButton(event, state.search_state, menu);
    }
    return false;
}

void MultiStyledMenuView::handleMouseScroll(MultiStyledMenuState &state, int scroll_amount,
                                             const domain::Menu &menu)
{
    switch (menu.getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu->handleMouseScroll(state.inline_state, scroll_amount, menu);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu->handleMouseScroll(state.prompt_state, scroll_amount, menu);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu->handleMouseScroll(state.search_state, scroll_amount, menu);
            break;
    }
}

void MultiStyledMenuView::ensureItemVisible(MultiStyledMenuState &state, int index, const domain::Menu &menu)
{
    switch (menu.getStyle()) {
        case domain::MenuStyle::INLINE:
            m_inline_menu->ensureItemVisible(state.inline_state, index);
            break;
        case domain::MenuStyle::PROMPT:
            m_prompt_menu->ensureItemVisible(state.prompt_state, index);
            break;
        case domain::MenuStyle::SEARCH:
            m_search_menu->ensureItemVisible(state.search_state, index);
            break;
    }
}

ObserverId MultiStyledMenuView::onMouseButton(std::function<void(int)> callback)
{
    ObserverId id = m_next_observer_id++;
    m_mouse_button_observers[id] = std::move(callback);
    return id;
}

void MultiStyledMenuView::removeObserver(ObserverId id)
{
    m_mouse_button_observers.erase(id);
}

float MultiStyledMenuView::x() const
{
    switch (m_last_style) {
        case domain::MenuStyle::INLINE: return m_inline_menu->x();
        case domain::MenuStyle::PROMPT: return m_prompt_menu->x();
        case domain::MenuStyle::SEARCH: return m_search_menu->x();
    }
    return 0;
}

float MultiStyledMenuView::y() const
{
    switch (m_last_style) {
        case domain::MenuStyle::INLINE: return m_inline_menu->y();
        case domain::MenuStyle::PROMPT: return m_prompt_menu->y();
        case domain::MenuStyle::SEARCH: return m_search_menu->y();
    }
    return 0;
}

float MultiStyledMenuView::width() const
{
    switch (m_last_style) {
        case domain::MenuStyle::INLINE: return m_inline_menu->width();
        case domain::MenuStyle::PROMPT: return m_prompt_menu->width();
        case domain::MenuStyle::SEARCH: return m_search_menu->width();
    }
    return 0;
}

float MultiStyledMenuView::height() const
{
    switch (m_last_style) {
        case domain::MenuStyle::INLINE: return m_inline_menu->height();
        case domain::MenuStyle::PROMPT: return m_prompt_menu->height();
        case domain::MenuStyle::SEARCH: return m_search_menu->height();
    }
    return 0;
}
