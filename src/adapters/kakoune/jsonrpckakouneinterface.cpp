#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "adapters/kakoune/color.hpp"
#include "domain/editor.hpp"
#include "domain/modeline.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/font.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include "domain/statusline.hpp"
#include "domain/uioptions.hpp"
#include "kakouneclientprocess.hpp"
#include "kakouneframestatemanager.hpp"
#include "spdlog/spdlog.h"
#include <memory>
#include <optional>

namespace kakoune {

JsonRpcKakouneInterface::JsonRpcKakouneInterface(const domain::KakouneSession& session, std::optional<std::string> startup_command, const std::vector<std::string>& file_arguments) : domain::KakouneInterface(session) {
    m_process = std::make_unique<KakouneClientProcess>(session.getSessionId());
    m_process->start(startup_command, file_arguments);

    m_frame_state_manager = std::make_unique<KakouneFrameStateManager>(m_process.get());

    m_frame_state_manager->start();
}

JsonRpcKakouneInterface::~JsonRpcKakouneInterface() {
    m_frame_state_manager->stop();
}

void JsonRpcKakouneInterface::setWakeEventLoopCallback(std::function<void()> callback) {
    m_frame_state_manager->setWakeEventLoopCallback(callback);
}

domain::KakouneState JsonRpcKakouneInterface::convertFrameStateToKakouneState(const FrameState& frame_state) {
    std::optional<domain::InfoBox> info_box;
    if (frame_state.info_box.has_value()) {
       info_box = domain::InfoBox{
            .title = toDomain(frame_state.info_box->title),
            .content = toDomain(frame_state.info_box->content),
            .anchor = toDomain(frame_state.info_box->anchor),
            .default_face = toDomain(frame_state.info_box->face),
            .style = toDomain(frame_state.info_box->style),
        };
    }

    std::optional<domain::StatusLine> status_line = domain::StatusLine(toDomain(frame_state.draw_status.status_line_prompt), toDomain(frame_state.draw_status.status_line_content));

    bool status_line_has_content = !(status_line->getPrompt().size() == 0 && status_line->getContent().size() == 0);

    std::optional<domain::Menu> menu;
    if (frame_state.menu.has_value()) {
        int menu_selected_index = (frame_state.menu_selected_index >= frame_state.menu->items.size()) ? -1 : frame_state.menu_selected_index;

        menu = domain::Menu(
            status_line.value(),
            domain::MenuItems(
                toDomain(frame_state.menu->items),
                toDomain(frame_state.menu->anchor),
                toDomain(frame_state.menu->face),
                toDomain(frame_state.menu->selected_face),
                menu_selected_index
            ),
            toDomain(frame_state.menu->style)
        );
    } else if (status_line_has_content) {
        menu = domain::Menu(status_line.value(), domain::MenuStyle::PROMPT);
    }

    if (!status_line_has_content) {
        status_line = std::nullopt;
    }

    domain::CursorPosition cursor_position;
    if (menu.has_value()) {
        cursor_position = domain::StatusLinePosition{ frame_state.draw_status.cursor_pos };
    }else {
        cursor_position = domain::BufferContentPosition{ domain::Coord{ frame_state.draw.cursor_pos.line, frame_state.draw.cursor_pos.column } };
    }

    return domain::KakouneState{
        .content = toDomain(frame_state.draw.lines),
        .cursor_position = cursor_position,
        .mode_line = domain::ModeLine(menu.has_value() ? std::nullopt : status_line, toDomain(frame_state.draw_status.mode_line), toDomain(frame_state.draw_status.default_face)),
        .info_box = info_box,
        .menu = menu,
        .default_face = toDomain(frame_state.draw.default_face),
    };
}

std::optional<domain::KakouneState> JsonRpcKakouneInterface::getNextKakouneState() {
    auto frame_state_opt = m_frame_state_manager->getNextFrameState();

    if (!frame_state_opt.has_value()) {
        return std::nullopt;
    }

    FrameState frame_state = frame_state_opt.value();

    if (frame_state.ui_options.has_value()) mergeUIOptions(m_ui_options, frame_state.ui_options.value());

    return convertFrameStateToKakouneState(frame_state);
}

domain::UIOptions JsonRpcKakouneInterface::getUIOptions(domain::FontManager* font_manager) {
    domain::UIOptions ui_options;

    if (m_ui_options.font.has_value()) {
        ui_options.font = font_manager->getFontFromName(m_ui_options.font.value());
        domain::loadBasicGlyphs(ui_options.font); // TODO check if the font is new
    }

    if (m_ui_options.color_border.has_value()) {
        ui_options.color_border = toDomain(m_ui_options.color_border.value());
    }

    if (m_ui_options.color_black.has_value()) {
        auto color = toDomain(m_ui_options.color_black.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Black] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_red.has_value()) {
        auto color = toDomain(m_ui_options.color_red.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Red] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_green.has_value()) {
        auto color = toDomain(m_ui_options.color_green.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Green] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_yellow.has_value()) {
        auto color = toDomain(m_ui_options.color_yellow.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Yellow] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_blue.has_value()) {
        auto color = toDomain(m_ui_options.color_blue.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Blue] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_magenta.has_value()) {
        auto color = toDomain(m_ui_options.color_magenta.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Magenta] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_cyan.has_value()) {
        auto color = toDomain(m_ui_options.color_cyan.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::Cyan] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_white.has_value()) {
        auto color = toDomain(m_ui_options.color_white.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::White] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_black.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_black.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightBlack] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_red.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_red.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightRed] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_green.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_green.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightGreen] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_yellow.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_yellow.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightYellow] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_blue.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_blue.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightBlue] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_magenta.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_magenta.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightMagenta] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_cyan.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_cyan.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightCyan] = std::get<domain::RGBAColor>(color);
        }
    }

    if (m_ui_options.color_bright_white.has_value()) {
        auto color = toDomain(m_ui_options.color_bright_white.value());
        if (std::holds_alternative<domain::RGBAColor>(color)) {
            ui_options.color_overrides[domain::FixedColor::BrightWhite] = std::get<domain::RGBAColor>(color);
        }
    }

    return ui_options;
}

domain::FrameEvents JsonRpcKakouneInterface::getEvents() {
    FrameEvents adapter_events = m_frame_state_manager->getEvents();

    return domain::FrameEvents{
        .menu_select = adapter_events.menu_select,
    };
}

std::optional<std::pair<domain::KakouneState, domain::FrameEvents>> JsonRpcKakouneInterface::getNextKakouneStateAndEvents() {
    auto result = m_frame_state_manager->getNextFrameStateAndEvents();

    if (!result.has_value()) {
        return std::nullopt;
    }

    FrameState frame_state = result->first;
    FrameEvents adapter_events = result->second;

    if (frame_state.ui_options.has_value()) mergeUIOptions(m_ui_options, frame_state.ui_options.value());

    domain::KakouneState kakoune_state = convertFrameStateToKakouneState(frame_state);

    domain::FrameEvents events{
        .menu_select = adapter_events.menu_select,
    };

    return std::make_pair(kakoune_state, events);
}

void JsonRpcKakouneInterface::pressKeys(const std::vector<std::string>& keys) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::KEYS,
        KeysRequestData{keys}
    });
}

void JsonRpcKakouneInterface::resize(int rows, int columns) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::RESIZE,
        ResizeRequestData{rows, columns}
    });
}

void JsonRpcKakouneInterface::scroll(int amount, int line, int column) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::SCROLL,
        ScrollRequestData{amount, line, column}
    });
}

void JsonRpcKakouneInterface::moveMouse(int line, int column) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::MOUSE_MOVE,
        MouseMoveRequestData{line, column}
    });
}

void JsonRpcKakouneInterface::pressMouseButton(domain::MouseButton button, int line, int column) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::MOUSE_PRESS,
        MousePressRequestData{getMouseButtonString(button), line, column}
    });
}

void JsonRpcKakouneInterface::releaseMouseButton(domain::MouseButton button, int line, int column) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::MOUSE_RELEASE,
        MouseReleaseRequestData{getMouseButtonString(button), line, column}
    });
}

void JsonRpcKakouneInterface::selectMenuItem(int index) {
    m_process->sendRequest(OutgoingRequest{
        OutgoingRequestType::MENU_SELECT,
        MenuSelectRequestData{index}
    });
}

std::string JsonRpcKakouneInterface::getMouseButtonString(domain::MouseButton button) {
    switch(button) {
        case domain::MouseButton::LEFT:
            return "left";
        case domain::MouseButton::MIDDLE:
            return "middle";
        case domain::MouseButton::RIGHT:
            return "right";
        default: {
            spdlog::warn("Can't convert MouseButton {} to kakoune button", (int)button);
            return "right";
        }
    }
}

void JsonRpcKakouneInterface::mergeUIOptions(UIOptions& original, UIOptions updated) {
    if (updated.font.has_value()) {
        original.font = updated.font.value();
    }

    if (updated.color_border.has_value()) {
        original.color_border = updated.color_border.value();
    }

    if (updated.color_black.has_value()) {
        original.color_black = updated.color_black.value();
    }

    if (updated.color_red.has_value()) {
        original.color_red = updated.color_red.value();
    }

    if (updated.color_green.has_value()) {
        original.color_green = updated.color_green.value();
    }

    if (updated.color_yellow.has_value()) {
        original.color_yellow = updated.color_yellow.value();
    }

    if (updated.color_blue.has_value()) {
        original.color_blue = updated.color_blue.value();
    }

    if (updated.color_magenta.has_value()) {
        original.color_magenta = updated.color_magenta.value();
    }

    if (updated.color_cyan.has_value()) {
        original.color_cyan = updated.color_cyan.value();
    }

    if (updated.color_white.has_value()) {
        original.color_white = updated.color_white.value();
    }

    if (updated.color_bright_black.has_value()) {
        original.color_bright_black = updated.color_bright_black.value();
    }

    if (updated.color_bright_red.has_value()) {
        original.color_bright_red = updated.color_bright_red.value();
    }

    if (updated.color_bright_green.has_value()) {
        original.color_bright_green = updated.color_bright_green.value();
    }

    if (updated.color_bright_yellow.has_value()) {
        original.color_bright_yellow = updated.color_bright_yellow.value();
    }

    if (updated.color_bright_blue.has_value()) {
        original.color_bright_blue = updated.color_bright_blue.value();
    }

    if (updated.color_bright_magenta.has_value()) {
        original.color_bright_magenta = updated.color_bright_magenta.value();
    }

    if (updated.color_bright_cyan.has_value()) {
        original.color_bright_cyan = updated.color_bright_cyan.value();
    }

    if (updated.color_bright_white.has_value()) {
        original.color_bright_white = updated.color_bright_white.value();
    }
}

}
