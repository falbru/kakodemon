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
