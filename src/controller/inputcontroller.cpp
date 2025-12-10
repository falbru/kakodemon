#include "controller/inputcontroller.hpp"
#include "core/keys.hpp"
#include "core/utf8string.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include <string>
#include <variant>
#include <spdlog/spdlog.h>

InputController::InputController() {

}

InputController::~InputController() {

}

void InputController::init(KakouneClient* kakoune_client, KakouneClientProcess*kakoune_process) {
    m_kakoune_client = kakoune_client;
    m_kakoune_process = kakoune_process;
}

void InputController::onKeyInput(const core::KeyEvent& event) {
    OutgoingRequest keys_request;
    keys_request.type = OutgoingRequestType::KEYS;
    keys_request.data = KeysRequestData{{keyEventToKakouneKey(event)}};

    m_kakoune_process->sendRequest(keys_request);
}

std::string InputController::keyEventToKakouneKey(const core::KeyEvent& event) {
    static const std::unordered_map<std::string, std::string> special_chars = {
        {" ", "space"},
        {"-", "minus"},
        {"+", "plus"},
        {"<", "lt"},
        {">", "gt"}
    };

    static const std::unordered_map<core::SpecialKey, std::string> special_keys = {
        {core::SpecialKey::ESCAPE,    "esc"},
        {core::SpecialKey::TAB,       "tab"},
        {core::SpecialKey::RETURN,    "ret"},
        {core::SpecialKey::BACKSPACE, "backspace"},
        {core::SpecialKey::DELETE,    "del"},
        {core::SpecialKey::LEFT,      "left"},
        {core::SpecialKey::RIGHT,     "right"},
        {core::SpecialKey::UP,        "up"},
        {core::SpecialKey::DOWN,      "down"},
        {core::SpecialKey::HOME,      "home"},
        {core::SpecialKey::END,       "end"},
        {core::SpecialKey::PAGE_UP,   "pageup"},
        {core::SpecialKey::PAGE_DOWN, "pagedown"},
        {core::SpecialKey::INSERT,    "ins"}
    };

    std::string key_name;

    bool has_modifiers = (event.modifiers & (core::CONTROL | core::ALT | core::SHIFT)) != 0;
    bool needsEscaping = false;

    if (std::holds_alternative<Codepoint>(event.key)) {
        key_name = codePointToString(std::get<Codepoint>(event.key));

        auto it = special_chars.find(key_name);
        if (it != special_chars.end()) {
            needsEscaping = true;
            key_name = it->second;
        }
    } else {
        core::SpecialKey special = std::get<core::SpecialKey>(event.key);
        needsEscaping = true;
        key_name = special_keys.at(special);
    }

    std::string result;
    if (needsEscaping || has_modifiers) {
        result = "<";
        if (event.modifiers & core::CONTROL) {
            result += "c-";
        }
        if (!std::holds_alternative<Codepoint>(event.key) && event.modifiers & core::SHIFT) {
            result += "s-";
        }
        if (event.modifiers & core::ALT) {
            result += "a-";
        }
        result += key_name + ">";
    }else {
        result = key_name;
    }

    return result;
}
