#include "inputcontroller.hpp"
#include "application/model/panelayout.hpp"
#include "domain/keys.hpp"
#include "domain/utf8string.hpp"
#include "spdlog/spdlog.h"
#include <algorithm>
#include <string>
#include <variant>

InputController::InputController() {

}

InputController::~InputController() {

}

void InputController::init(FocusedClientStack *focused_client_stack, ClientManager *client_manager, domain::Window *window, PaneLayout *pane_layout) {
    m_focused_client_stack = focused_client_stack;
    m_client_manager = client_manager;
    m_pane_layout = pane_layout;
    m_window = window;

    window->onKeyInput([this](const domain::KeyEvent& event) {
        onKeyInput(event);
    });
}

void InputController::onKeyInput(const domain::KeyEvent& event) {
    if (tryHandleGlobalKeybinding(event)) {
        return;
    }
    m_pending_keys.push_back(keyEventToKakouneKey(event));
}

bool InputController::tryHandleGlobalKeybinding(const domain::KeyEvent &event) {
    bool ctrl_shift = (event.modifiers & (domain::CONTROL | domain::SHIFT)) == (domain::CONTROL | domain::SHIFT);
    if (!ctrl_shift) {
        return false;
    }

    if (std::holds_alternative<domain::SpecialKey>(event.key)) {
        domain::SpecialKey sk = std::get<domain::SpecialKey>(event.key);

        if (sk == domain::SpecialKey::RETURN) {
            if (m_pane_layout->getLayoutType() == LayoutType::FULL) {
                m_pane_layout->setLayoutType(m_pane_layout->getPreviousLayoutType());
            }else {
                m_pane_layout->setLayoutType(LayoutType::FULL);
            }
            m_pane_layout->arrange();
            return true;
        }

        return false;
    }

    auto cp = std::get<domain::Codepoint>(event.key);
    std::string key = domain::codePointToString(cp);

    if (key == "j" || key == "J") {
        focusNext();
        return true;
    }
    if (key == "k" || key == "K") {
        focusPrev();
        return true;
    }
    if (key == "h" || key == "H") {
        m_pane_layout->setMasterRatio(m_pane_layout->getMasterRatio() - 0.05f);
        m_pane_layout->arrange();
        m_window->setNeedsRerender();
        return true;
    }
    if (key == "l" || key == "L") {
        m_pane_layout->setMasterRatio(m_pane_layout->getMasterRatio() + 0.05f);
        m_pane_layout->arrange();
        m_window->setNeedsRerender();
        return true;
    }
    if (key == "i" || key == "I") {
        m_pane_layout->setNumMasters(m_pane_layout->getNumMasters() + 1);
        m_pane_layout->arrange();
        m_window->setNeedsRerender();
        return true;
    }
    if (key == "d" || key == "D") {
        m_pane_layout->setNumMasters(m_pane_layout->getNumMasters() - 1);
        m_pane_layout->arrange();
        m_window->setNeedsRerender();
        return true;
    }
    return false;
}

void InputController::focusNext() {
    const auto &clients = m_client_manager->clients();
    if (clients.empty()) return;

    KakouneClient *current = m_focused_client_stack->focused();
    auto it = std::find_if(clients.begin(), clients.end(),
                           [current](const auto &c) { return c.get() == current; });

    if (it == clients.end() || std::next(it) == clients.end()) {
        m_focused_client_stack->focus(clients.front().get());
    } else {
        m_focused_client_stack->focus(std::next(it)->get());
    }
}

void InputController::focusPrev() {
    const auto &clients = m_client_manager->clients();
    if (clients.empty()) return;

    KakouneClient *current = m_focused_client_stack->focused();
    auto it = std::find_if(clients.begin(), clients.end(),
                           [current](const auto &c) { return c.get() == current; });

    if (it == clients.end() || it == clients.begin()) {
        m_focused_client_stack->focus(clients.back().get());
    } else {
        m_focused_client_stack->focus(std::prev(it)->get());
    }
}

void InputController::update() {
    if (!m_pending_keys.empty() && m_focused_client_stack->focused()) {
        m_focused_client_stack->focused()->interface->pressKeys(m_pending_keys);
        m_pending_keys.clear();
    }
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
        {domain::SpecialKey::INSERT,    "ins"},
        {domain::SpecialKey::F1,        "F1"},
        {domain::SpecialKey::F2,        "F2"},
        {domain::SpecialKey::F3,        "F3"},
        {domain::SpecialKey::F4,        "F4"},
        {domain::SpecialKey::F5,        "F5"},
        {domain::SpecialKey::F6,        "F6"},
        {domain::SpecialKey::F7,        "F7"},
        {domain::SpecialKey::F8,        "F8"},
        {domain::SpecialKey::F9,        "F9"},
        {domain::SpecialKey::F10,       "F10"},
        {domain::SpecialKey::F11,       "F11"},
        {domain::SpecialKey::F12,       "F12"},
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
