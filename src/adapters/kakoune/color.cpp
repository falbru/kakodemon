#include "color.hpp"
#include "domain/color.hpp"
#include <spdlog/spdlog.h>

void kakoune::to_json(nlohmann::json &j, const Color &c)
{
    j = c.color_string;
}

void kakoune::from_json(const nlohmann::json &j, Color &c)
{
    c.color_string = j.get<std::string>();
}

int hexToInt(const std::string &hex)
{
    int result = 0;
    for (char c : hex)
    {
        result *= 16;
        if (c >= '0' && c <= '9')
        {
            result += c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            result += c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            result += c - 'A' + 10;
        }
    }
    return result;
}

bool IsValidHexChar(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

domain::OptionalColor kakoune::toDomain(Color c)
{
    const std::string &color_string = c.color_string;

    if (color_string == "default")
    {
        return domain::DefaultColor();
    }

    static const std::unordered_map<std::string, domain::NamedColor> NAMED_COLORS = {
        {"black", domain::NamedColor::Black},
        {"red", domain::NamedColor::Red},
        {"green", domain::NamedColor::Green},
        {"yellow", domain::NamedColor::Yellow},
        {"blue", domain::NamedColor::Blue},
        {"magenta", domain::NamedColor::Magenta},
        {"cyan", domain::NamedColor::Cyan},
        {"white", domain::NamedColor::White},
        {"bright-black", domain::NamedColor::BrightBlack},
        {"bright-red", domain::NamedColor::BrightRed},
        {"bright-green", domain::NamedColor::BrightGreen},
        {"bright-yellow", domain::NamedColor::BrightYellow},
        {"bright-blue", domain::NamedColor::BrightBlue},
        {"bright-magenta", domain::NamedColor::BrightMagenta},
        {"bright-cyan", domain::NamedColor::BrightCyan},
        {"bright-white", domain::NamedColor::BrightWhite},
    };

    auto named_it = NAMED_COLORS.find(color_string);
    if (named_it != NAMED_COLORS.end())
    {
        return named_it->second;
    }

    // rgb:rrggbb
    if (color_string.length() == 10 && color_string.substr(0, 4) == "rgb:")
    {
        std::string hex_color = color_string.substr(4);

        for (char c : hex_color)
        {
            if (!IsValidHexChar(c))
            {
                throw ColorConversionException(color_string);
            }
        }

        int red_int = hexToInt(hex_color.substr(0, 2));
        int green_int = hexToInt(hex_color.substr(2, 2));
        int blue_int = hexToInt(hex_color.substr(4, 2));

        float red = red_int / 255.0f;
        float green = green_int / 255.0f;
        float blue = blue_int / 255.0f;

        return domain::RGBAColor{red, green, blue, 1.0f};
    }

    // rgba:rrggbbaa
    if (color_string.length() == 13 && color_string.substr(0, 5) == "rgba:")
    {
        std::string hex_color = color_string.substr(5);

        for (char c : hex_color)
        {
            if (!IsValidHexChar(c))
            {
                throw ColorConversionException(color_string);
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

        return domain::RGBAColor{red, green, blue, alpha};
    }

    throw ColorConversionException(color_string);
}
