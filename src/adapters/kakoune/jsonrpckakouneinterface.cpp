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

namespace {

void applyColorOverride(
    std::unordered_map<domain::FixedColor, domain::RGBAColor>& overrides,
    domain::FixedColor fixed_color,
    const Color& color)
{
    try {
        auto converted = toDomain(color);
        if (std::holds_alternative<domain::RGBAColor>(converted)) {
            overrides[fixed_color] = std::get<domain::RGBAColor>(converted);
        } else if (std::holds_alternative<domain::DefaultColor>(converted)) {
            overrides[fixed_color] = domain::getRGBAColor(fixed_color);
        }
    } catch (const ColorConversionException& e) {
        overrides[fixed_color] = domain::getRGBAColor(fixed_color);
    }
}

}

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

    bool status_line_has_prompt = status_line->getPrompt().size() > 0;

    std::optional<domain::Menu> menu;
    if (frame_state.menu.has_value()) {
        int menu_selected_index = (frame_state.menu_selected_index >= frame_state.menu->items.size()) ? -1 : frame_state.menu_selected_index;

        menu = domain::Menu(
            status_line.value(),
            toDomain(frame_state.draw_status.default_face),
            domain::MenuItems(
                toDomain(frame_state.menu->items),
                toDomain(frame_state.menu->anchor),
                toDomain(frame_state.menu->face),
                toDomain(frame_state.menu->selected_face),
                menu_selected_index
            ),
            toDomain(frame_state.menu->style)
        );
    } else if (status_line_has_prompt) {
        menu = domain::Menu(status_line.value(), toDomain(frame_state.draw_status.default_face), domain::MenuStyle::PROMPT);
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
        .mode_line = domain::ModeLine((menu.has_value() || status_line->getContent().size() == 0) ? std::nullopt : status_line, toDomain(frame_state.draw_status.mode_line), toDomain(frame_state.draw_status.default_face)),
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

    if (frame_state.ui_options.has_value()) m_ui_options = frame_state.ui_options.value();

    return convertFrameStateToKakouneState(frame_state);
}

domain::UIOptions JsonRpcKakouneInterface::getUIOptions(domain::FontManager* font_manager) {
    domain::UIOptions ui_options;

    if (m_ui_options.font.has_value()) {
        ui_options.font = font_manager->getFontFromName(m_ui_options.font.value());
        domain::loadBasicGlyphs(ui_options.font); // TODO check if the font is new
    }else {
        ui_options.font = font_manager->getDefaultFont(14);
        domain::loadBasicGlyphs(ui_options.font);
    }

    if (m_ui_options.font_menu.has_value()) {
        ui_options.font_menu = font_manager->getFontFromName(m_ui_options.font_menu.value());
        domain::loadBasicGlyphs(ui_options.font_menu);
    } else {
        ui_options.font_menu = ui_options.font;
    }

    if (m_ui_options.font_infobox.has_value()) {
        ui_options.font_infobox = font_manager->getFontFromName(m_ui_options.font_infobox.value());
        domain::loadBasicGlyphs(ui_options.font_infobox);
    } else {
        ui_options.font_infobox = ui_options.font;
    }

    if (m_ui_options.font_statusbar.has_value()) {
        ui_options.font_statusbar = font_manager->getFontFromName(m_ui_options.font_statusbar.value());
        domain::loadBasicGlyphs(ui_options.font_statusbar);
    } else {
        ui_options.font_statusbar = ui_options.font;
    }

    if (m_ui_options.font_content.has_value()) {
        ui_options.font_content = font_manager->getFontFromName(m_ui_options.font_content.value());
        domain::loadBasicGlyphs(ui_options.font_content);
    } else {
        ui_options.font_content = ui_options.font;
    }

    if (m_ui_options.color_border.has_value()) {
        try {
            ui_options.color_border = toDomain(m_ui_options.color_border.value());
        } catch (const ColorConversionException& e) {
            spdlog::warn("{}", e.what());
        }
    }

    if (m_ui_options.color_black.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Black, m_ui_options.color_black.value());
    if (m_ui_options.color_red.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Red, m_ui_options.color_red.value());
    if (m_ui_options.color_green.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Green, m_ui_options.color_green.value());
    if (m_ui_options.color_yellow.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Yellow, m_ui_options.color_yellow.value());
    if (m_ui_options.color_blue.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Blue, m_ui_options.color_blue.value());
    if (m_ui_options.color_magenta.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Magenta, m_ui_options.color_magenta.value());
    if (m_ui_options.color_cyan.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::Cyan, m_ui_options.color_cyan.value());
    if (m_ui_options.color_white.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::White, m_ui_options.color_white.value());
    if (m_ui_options.color_bright_black.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightBlack, m_ui_options.color_bright_black.value());
    if (m_ui_options.color_bright_red.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightRed, m_ui_options.color_bright_red.value());
    if (m_ui_options.color_bright_green.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightGreen, m_ui_options.color_bright_green.value());
    if (m_ui_options.color_bright_yellow.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightYellow, m_ui_options.color_bright_yellow.value());
    if (m_ui_options.color_bright_blue.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightBlue, m_ui_options.color_bright_blue.value());
    if (m_ui_options.color_bright_magenta.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightMagenta, m_ui_options.color_bright_magenta.value());
    if (m_ui_options.color_bright_cyan.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightCyan, m_ui_options.color_bright_cyan.value());
    if (m_ui_options.color_bright_white.has_value())
        applyColorOverride(ui_options.color_overrides, domain::FixedColor::BrightWhite, m_ui_options.color_bright_white.value());

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

    if (frame_state.ui_options.has_value()) m_ui_options = frame_state.ui_options.value();

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

bool JsonRpcKakouneInterface::isClientRunning() {
    return m_process->isClientRunning();
}

}
