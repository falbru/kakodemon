#include "focusedclientstack.hpp"
#include <algorithm>

KakouneClient *FocusedClientStack::focused() const {
    return m_stack.empty() ? nullptr : m_stack.back();
}

void FocusedClientStack::focus(KakouneClient *client) {
    auto it = std::find(m_stack.begin(), m_stack.end(), client);
    if (it != m_stack.end()) {
        m_stack.erase(it);
    }
    m_stack.push_back(client);
    m_focus_changed_observers.notify(client);
}

void FocusedClientStack::remove(KakouneClient *client) {
    auto it = std::find(m_stack.begin(), m_stack.end(), client);
    if (it == m_stack.end()) return;

    bool was_focused = (it == m_stack.end() - 1);
    m_stack.erase(it);

    if (was_focused) {
        m_focus_changed_observers.notify(focused());
    }
}

ObserverId FocusedClientStack::onFocusChanged(std::function<void(KakouneClient *)> callback) {
    return m_focus_changed_observers.addObserver(std::move(callback));
}

void FocusedClientStack::removeObserver(ObserverId id) {
    m_focus_changed_observers.removeObserver(id);
}
