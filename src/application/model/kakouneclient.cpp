#include "application/model/kakouneclient.hpp"
#include "domain/color.hpp"
#include "domain/editor.hpp"
#include "domain/face.hpp"
#include "domain/modeline.hpp"
#include "domain/ports/kakouneinterface.hpp"

domain::KakouneState getEmptyState() {
    return {
        domain::Lines(),
        domain::BufferContentPosition(),
        domain::ModeLine(std::nullopt, domain::Line(), domain::Face(domain::DefaultColor(), domain::DefaultColor())),
        std::nullopt,
        std::nullopt,
        domain::Face(domain::DefaultColor(), domain::DefaultColor())
    };
}

KakouneClient::KakouneClient(domain::KakouneSession* session, std::unique_ptr<domain::KakouneInterface> interface)
    : session(session), interface(std::move(interface)), state(getEmptyState()) {
}

const domain::UIOptions &KakouneClient::uiOptions() const {
    return m_ui_options;
}

void KakouneClient::setUIOptions(domain::UIOptions ui_options) {
    if (m_ui_options == ui_options) return;
    m_ui_options = std::move(ui_options);
    m_ui_options_observers.notify(m_ui_options);
}

ObserverId KakouneClient::onUIOptionsChanged(std::function<void(const domain::UIOptions &)> callback) {
    return m_ui_options_observers.addObserver(std::move(callback));
}

void KakouneClient::removeUIOptionsObserver(ObserverId id) {
    m_ui_options_observers.removeObserver(id);
}
