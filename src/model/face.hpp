#ifndef FACE_HPP
#define FACE_HPP

#include <string>

#include "nlohmann/json.hpp"

struct Face
{
    std::string fg;
    std::string bg;
};

void to_json(nlohmann::json &j, const Face &f);
void from_json(const nlohmann::json &j, Face &f);

#endif
