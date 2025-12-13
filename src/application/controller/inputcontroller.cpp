#include "inputcontroller.hpp"
#include "domain/keys.hpp"
#include "domain/utf8string.hpp"
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

void InputController::onKeyInput(const domain::KeyEvent& event) {
    OutgoingRequest keys_request;
    keys_request.type = OutgoingRequestType::KEYS;
    keys_request.data = KeysRequestData{{keyEventToKakouneKey(event)}};

    m_kakoune_process->sendRequest(keys_request);
}

std::string InputController::keyEventToKakouneKey(const domain::KeyEvent& event) {
    static const std::unordered_map<std::string, std::string> special_chars = {
        {" ", "space"},
        {"-", "minus"},
        {"+", "plus"},
        {"<", "lt"},
        {">", "gt"}
    };

    static const std::unordered_map<domain::SpecialKey, std::string> special_keys = {
        {domain::SpecialKey::ESCAPE,    "esc"},
        {domain::SpecialKey::TAB,       "tab"},
        {domain::SpecialKey::RETURN,    "ret"},
        {domain::SpecialKey::BACKSPACE, "backspace"},
        {domain::SpecialKey::DELETE,    "del"},
        {domain::SpecialKey::LEFT,      "left"},
        {domain::SpecialKey::RIGHT,     "right"},
        {domain::SpecialKey::UP,        "up"},
        {domain::SpecialKey::DOWN,      "down"},
        {domain::SpecialKey::HOME,      "home"},
        {domain::SpecialKey::END,       "end"},
        {domain::SpecialKey::PAGE_UP,   "pageup"},
        {domain::SpecialKey::PAGE_DOWN, "pagedown"},
        {domain::SpecialKey::INSERT,    "ins"}
    };

    std::string key_name;

    bool has_modifiers = (event.modifiers & (domain::CONTROL | domain::ALT | domain::SHIFT)) != 0;
    bool needsEscaping = false;

    if (std::holds_alternative<domain::Codepoint>(event.key)) {
        key_name = domain::codePointToString(std::get<domain::Codepoint>(event.key));

        auto it = special_chars.find(key_name);
        if (it != special_chars.end()) {
            needsEscaping = true;
            key_name = it->second;
        }
    } else {
        domain::SpecialKey special = std::get<domain::SpecialKey>(event.key);
        needsEscaping = true;
        key_name = special_keys.at(special);
    }

    std::string result;
    if (needsEscaping || has_modifiers) {
        result = "<";
        if (event.modifiers & domain::CONTROL) {
            result += "c-";
        }
        if (!std::holds_alternative<domain::Codepoint>(event.key) && event.modifiers & domain::SHIFT) {
            result += "s-";
        }
        if (event.modifiers & domain::ALT) {
            result += "a-";
        }
        result += key_name + ">";
    }else {
        result = key_name;
    }

    return result;
}
