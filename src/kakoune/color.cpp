#include "color.hpp"
#include "domain/color.hpp"
#include <cctype>
#include <spdlog/spdlog.h>

const std::unordered_map<std::string, domain::Color> NAMED_COLORS = {
    {"black",          domain::Color{0.0f, 0.0f, 0.0f, 1.0f}},
    {"red",            domain::Color{1.0f, 0.0f, 0.0f, 1.0f}},
    {"green",          domain::Color{0.0f, 0.502f, 0.0f, 1.0f}},
    {"yellow",         domain::Color{1.0f, 1.0f, 0.0f, 1.0f}},
    {"blue",           domain::Color{0.0f, 0.0f, 1.0f, 1.0f}},
    {"magenta",        domain::Color{1.0f, 0.0f, 1.0f, 1.0f}},
    {"cyan",           domain::Color{0.0f, 1.0f, 1.0f, 1.0f}},
    {"white",          domain::Color{1.0f, 1.0f, 1.0f, 1.0f}},
    {"bright-black",   domain::Color{0.502f, 0.502f, 0.502f, 1.0f}},
    {"bright-red",     domain::Color{1.0f, 0.502f, 0.502f, 1.0f}},
    {"bright-green",   domain::Color{0.502f, 1.0f, 0.502f, 1.0f}},
    {"bright-yellow",  domain::Color{1.0f, 1.0f, 0.502f, 1.0f}},
    {"bright-blue",    domain::Color{0.502f, 0.502f, 1.0f, 1.0f}},
    {"bright-magenta", domain::Color{1.0f, 0.502f, 1.0f, 1.0f}},
    {"bright-cyan",    domain::Color{0.502f, 1.0f, 1.0f, 1.0f}},
    {"bright-white",   domain::Color{1.0f, 1.0f, 1.0f, 1.0f}},
};

const domain::Color default_fg_color = domain::Color{1.0f, 1.0f, 1.0f, 1.0f};
const domain::Color default_bg_color = domain::Color{0.0f, 0.0f, 0.0f, 1.0f};

int hexToInt(const std::string& hex) {
    int result = 0;
    for (char c : hex) {
        result *= 16;
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        }
    }
    return result;
}

bool IsValidHexChar(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

domain::Color kakoune::Color::toCoreColor(std::optional<Color> default_color, bool fg) const {
    if (color_string == "default") {
        if (default_color.has_value()) {
            return default_color.value().toCoreColor(std::nullopt, fg);
        }else {
            if (fg) return default_fg_color;
            else return default_bg_color;
        }
    }

    auto named_it = NAMED_COLORS.find(color_string);
    if (named_it != NAMED_COLORS.end()) {
        return named_it->second;
    }

    // rgb:rrggbb
    if (color_string.length() == 10 && color_string.substr(0, 4) == "rgb:") {
        std::string hex_color = color_string.substr(4);

        for (char c : hex_color) {
            if (!IsValidHexChar(c)) {
                throw std::runtime_error("Invalid hex characters in rgb: format: " + color_string);
            }
        }

        int red_int = hexToInt(hex_color.substr(0, 2));
        int green_int = hexToInt(hex_color.substr(2, 2));
        int blue_int = hexToInt(hex_color.substr(4, 2));

        float red = red_int / 255.0f;
        float green = green_int / 255.0f;
        float blue = blue_int / 255.0f;

        return domain::Color{red, green, blue, 1.0f};
    }

    // rgba:rrggbbaa
    if (color_string.length() == 13 && color_string.substr(0, 5) == "rgba:") {
        std::string hex_color = color_string.substr(5);

        for (char c : hex_color) {
            if (!IsValidHexChar(c)) {
                throw std::runtime_error("Invalid hex characters in rgba: format: " + color_string);
            }
        }

        int red_int = hexToInt(hex_color.substr(0, 2));
        int green_int = hexToInt(hex_color.substr(2, 2));
        int blue_int = hexToInt(hex_color.substr(4, 2));
        int alpha_int = hexToInt(hex_color.substr(6, 2));

        float red = red_int / 255.0f;
        float green = green_int / 255.0f;
        float blue = blue_int / 255.0f;
        float alpha = alpha_int / 255.0f;

        return domain::Color{red, green, blue, alpha};
    }

    throw std::runtime_error("Unable to convert color: " + color_string);
}

void kakoune::to_json(nlohmann::json &j, const Color &c)
{
    j = c.color_string;
}

void kakoune::from_json(const nlohmann::json &j, Color &c)
{
    c.color_string = j.get<std::string>();
}
