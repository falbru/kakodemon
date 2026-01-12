#include "uioptions.hpp"

namespace kakoune {

void to_json(nlohmann::json &j, const UIOptions &ui_options) {
    if (ui_options.font.has_value()) {
        j["kakodemon_set_font"] = ui_options.font.value();
    }
    if (ui_options.font.has_value()) {
        j["kakodemon_set_color_border"] = ui_options.color_border->color_string;
    }
}

void from_json(const nlohmann::json &j, UIOptions &ui_options) {
    if (j.contains("kakodemon_set_font")) {
        ui_options.font = j["kakodemon_set_font"];
    }
    if (j.contains("kakodemon_set_color_border")) {
        ui_options.color_border = Color{j["kakodemon_set_color_border"]};
    }
}

}
