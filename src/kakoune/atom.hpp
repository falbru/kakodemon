#ifndef ATOM_HPP_INCLUDED
#define ATOM_HPP_INCLUDED

#include "face.hpp"
#include "core/utf8string.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Atom
{
    Face face;
    UTF8String contents;
};

void to_json(nlohmann::json &j, const Atom &atom);
void from_json(const nlohmann::json &j, Atom &atom);

} // namespace kakoune

#endif
