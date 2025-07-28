#ifndef FACE_HPP
#define FACE_HPP

#include "kakoune/color.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Face
{
    Color fg;
    Color bg;
};

void to_json(nlohmann::json &j, const Face &f);
void from_json(const nlohmann::json &j, Face &f);

} // namespace kakoune

#endif
