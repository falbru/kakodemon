#ifndef FACE_HPP
#define FACE_HPP

#include "color.hpp"
#include "domain/face.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

enum Attribute
{
    UNDERLINE,
    CURLY_UNDERLINE,
    DOUBLE_UNDERLINE,
    REVERSE,
    BLINK,
    BOLD,
    DIM,
    ITALIC,
    FINAL_FG,
    FINAL_BG,
    FINAL_ATTR
};

struct Face
{
    Color fg;
    Color bg;
    std::vector<Attribute> attributes;
};

void to_json(nlohmann::json &j, const Face &f);
void from_json(const nlohmann::json &j, Face &f);

void to_json(nlohmann::json &j, const Attribute &a);
void from_json(const nlohmann::json &j, Attribute &a);

domain::Face toDomain(kakoune::Face face);

} // namespace kakoune

#endif
