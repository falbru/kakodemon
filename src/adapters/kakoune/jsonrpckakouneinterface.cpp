#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "domain/editor.hpp"
#include "domain/modeline.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include "domain/statusline.hpp"
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

std::optional<domain::KakouneState> JsonRpcKakouneInterface::getNextKakouneState() {
    auto frame_state_opt = m_frame_state_manager->getNextFrameState();

    if (!frame_state_opt.has_value()) {
        return std::nullopt;
    }

    FrameState frame_state = frame_state_opt.value();

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

    std::optional<domain::Menu> menu;
    if (frame_state.menu.has_value()) {
        menu = domain::Menu(
            toDomain(frame_state.menu->items),
            frame_state.menu_selected_index,
            status_line.value(),
            toDomain(frame_state.menu->anchor),
            toDomain(frame_state.menu->face),
            toDomain(frame_state.menu->selected_face),
            toDomain(frame_state.menu->style)
        );
    }

    if (status_line->getPrompt().size() == 0 && status_line->getContent().size() == 0) {
        status_line = std::nullopt;
    }

    domain::CursorPosition cursor_position;
    if (frame_state.menu.has_value()) {
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

}
