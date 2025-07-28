#include "controller/inputcontroller.hpp"
#include "kakoune/kakouneclientprocess.hpp"
#include <string>
#include <variant>
#include <spdlog/spdlog.h>

InputController::InputController() {

}

InputController::~InputController() {

}

void InputController::init(std::shared_ptr<KakouneClient> kakoune_client, std::shared_ptr<KakouneClientProcess> kakoune_process) {
    m_kakoune_client = kakoune_client;
    m_kakoune_process = kakoune_process;
}

void InputController::onKeyInput(const KeyEvent& event) {
    OutgoingRequest keys_request;
    keys_request.type = OutgoingRequestType::KEYS;
    keys_request.data = KeysRequestData{{keyEventToKakouneKey(event)}};

    m_kakoune_process->sendRequest(keys_request);
}

std::string InputController::keyEventToKakouneKey(const KeyEvent& event) {
    static const std::unordered_map<std::string, std::string> special_chars = {
        {" ", "space"},
        {"-", "minus"},
        {"+", "plus"},
        {"<", "lt"},
        {">", "gt"}
    };

    static const std::unordered_map<SpecialKey, std::string> special_keys = {
        {ESCAPE, "esc"},
        {TAB, "tab"},
        {RETURN, "ret"},
        {BACKSPACE, "backspace"},
        {DELETE, "del"},
        {LEFT, "left"},
        {RIGHT, "right"},
        {UP, "up"},
        {DOWN, "down"},
        {HOME, "home"},
        {END, "end"},
        {PAGE_UP, "pageup"},
        {PAGE_DOWN, "pagedown"},
        {INSERT, "ins"}
    };

    std::string key_name;

    bool has_modifiers = (event.modifiers & (CONTROL | ALT | SHIFT)) != 0;
    bool needsEscaping = false;

    if (std::holds_alternative<std::string>(event.key)) {
        key_name = std::get<std::string>(event.key);

        auto it = special_chars.find(key_name);
        if (it != special_chars.end()) {
            needsEscaping = true;
            key_name = it->second;
        }
    } else {
        SpecialKey special = std::get<SpecialKey>(event.key);
        needsEscaping = true;
        key_name = special_keys.at(special);
    }

    std::string result;
    if (needsEscaping || has_modifiers) {
        result = "<";
        if (event.modifiers & CONTROL) {
            result += "c-";
        }
        if (!std::holds_alternative<std::string>(event.key) && event.modifiers & SHIFT) {
            result += "s-";
        }
        if (event.modifiers & ALT) {
            result += "a-";
        }
        result += key_name + ">";
    }else {
        result = key_name;
    }

    return result;
}
