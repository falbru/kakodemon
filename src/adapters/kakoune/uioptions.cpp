#include "uioptions.hpp"

namespace kakoune {

void to_json(nlohmann::json &j, const UIOptions &ui_options) {
    if (ui_options.font.has_value()) {
        j["kakodemon_set_font"] = ui_options.font.value();
    }
    if (ui_options.color_border.has_value()) {
        j["kakodemon_set_color_border"] = ui_options.color_border->color_string;
    }
    if (ui_options.color_black.has_value()) {
        j["kakodemon_set_color_black"] = ui_options.color_black->color_string;
    }
    if (ui_options.color_red.has_value()) {
        j["kakodemon_set_color_red"] = ui_options.color_red->color_string;
    }
    if (ui_options.color_green.has_value()) {
        j["kakodemon_set_color_green"] = ui_options.color_green->color_string;
    }
    if (ui_options.color_yellow.has_value()) {
        j["kakodemon_set_color_yellow"] = ui_options.color_yellow->color_string;
    }
    if (ui_options.color_blue.has_value()) {
        j["kakodemon_set_color_blue"] = ui_options.color_blue->color_string;
    }
    if (ui_options.color_magenta.has_value()) {
        j["kakodemon_set_color_magenta"] = ui_options.color_magenta->color_string;
    }
    if (ui_options.color_cyan.has_value()) {
        j["kakodemon_set_color_cyan"] = ui_options.color_cyan->color_string;
    }
    if (ui_options.color_white.has_value()) {
        j["kakodemon_set_color_white"] = ui_options.color_white->color_string;
    }
    if (ui_options.color_bright_black.has_value()) {
        j["kakodemon_set_color_bright_black"] = ui_options.color_bright_black->color_string;
    }
    if (ui_options.color_bright_red.has_value()) {
        j["kakodemon_set_color_bright_red"] = ui_options.color_bright_red->color_string;
    }
    if (ui_options.color_bright_green.has_value()) {
        j["kakodemon_set_color_bright_green"] = ui_options.color_bright_green->color_string;
    }
    if (ui_options.color_bright_yellow.has_value()) {
        j["kakodemon_set_color_bright_yellow"] = ui_options.color_bright_yellow->color_string;
    }
    if (ui_options.color_bright_blue.has_value()) {
        j["kakodemon_set_color_bright_blue"] = ui_options.color_bright_blue->color_string;
    }
    if (ui_options.color_bright_magenta.has_value()) {
        j["kakodemon_set_color_bright_magenta"] = ui_options.color_bright_magenta->color_string;
    }
    if (ui_options.color_bright_cyan.has_value()) {
        j["kakodemon_set_color_bright_cyan"] = ui_options.color_bright_cyan->color_string;
    }
    if (ui_options.color_bright_white.has_value()) {
        j["kakodemon_set_color_bright_white"] = ui_options.color_bright_white->color_string;
    }
}

void from_json(const nlohmann::json &j, UIOptions &ui_options) {
    if (j.contains("kakodemon_set_font")) {
        ui_options.font = j["kakodemon_set_font"];
    }
    if (j.contains("kakodemon_set_color_border")) {
        ui_options.color_border = Color{j["kakodemon_set_color_border"]};
    }
    if (j.contains("kakodemon_set_color_black")) {
        ui_options.color_black = Color{j["kakodemon_set_color_black"]};
    }
    if (j.contains("kakodemon_set_color_red")) {
        ui_options.color_red = Color{j["kakodemon_set_color_red"]};
    }
    if (j.contains("kakodemon_set_color_green")) {
        ui_options.color_green = Color{j["kakodemon_set_color_green"]};
    }
    if (j.contains("kakodemon_set_color_yellow")) {
        ui_options.color_yellow = Color{j["kakodemon_set_color_yellow"]};
    }
    if (j.contains("kakodemon_set_color_blue")) {
        ui_options.color_blue = Color{j["kakodemon_set_color_blue"]};
    }
    if (j.contains("kakodemon_set_color_magenta")) {
        ui_options.color_magenta = Color{j["kakodemon_set_color_magenta"]};
    }
    if (j.contains("kakodemon_set_color_cyan")) {
        ui_options.color_cyan = Color{j["kakodemon_set_color_cyan"]};
    }
    if (j.contains("kakodemon_set_color_white")) {
        ui_options.color_white = Color{j["kakodemon_set_color_white"]};
    }
    if (j.contains("kakodemon_set_color_bright_black")) {
        ui_options.color_bright_black = Color{j["kakodemon_set_color_bright_black"]};
    }
    if (j.contains("kakodemon_set_color_bright_red")) {
        ui_options.color_bright_red = Color{j["kakodemon_set_color_bright_red"]};
    }
    if (j.contains("kakodemon_set_color_bright_green")) {
        ui_options.color_bright_green = Color{j["kakodemon_set_color_bright_green"]};
    }
    if (j.contains("kakodemon_set_color_bright_yellow")) {
        ui_options.color_bright_yellow = Color{j["kakodemon_set_color_bright_yellow"]};
    }
    if (j.contains("kakodemon_set_color_bright_blue")) {
        ui_options.color_bright_blue = Color{j["kakodemon_set_color_bright_blue"]};
    }
    if (j.contains("kakodemon_set_color_bright_magenta")) {
        ui_options.color_bright_magenta = Color{j["kakodemon_set_color_bright_magenta"]};
    }
    if (j.contains("kakodemon_set_color_bright_cyan")) {
        ui_options.color_bright_cyan = Color{j["kakodemon_set_color_bright_cyan"]};
    }
    if (j.contains("kakodemon_set_color_bright_white")) {
        ui_options.color_bright_white = Color{j["kakodemon_set_color_bright_white"]};
    }
}

}
