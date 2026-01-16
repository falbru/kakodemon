#ifndef KAKOUNE_COLOR_HPP_INCLUDED
#define KAKOUNE_COLOR_HPP_INCLUDED

#include <stdexcept>
#include <string>

#include "domain/color.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

class ColorConversionException : public std::exception
{
  public:
    explicit ColorConversionException(const std::string &color_string)
        : m_message("Unable to convert color: " + color_string)
    {
    }

    const char *what() const noexcept override
    {
        return m_message.c_str();
    }

  private:
    std::string m_message;
};

struct Color
{
    std::string color_string;
};

void to_json(nlohmann::json &j, const Color &c);
void from_json(const nlohmann::json &j, Color &c);

domain::OptionalColor toDomain(Color c);

} // namespace kakoune

#endif
