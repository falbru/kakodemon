#ifndef ATOM_HPP_INCLUDED
#define ATOM_HPP_INCLUDED

#include "face.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Atom
{
    Face face;
    std::string contents;
};

void to_json(nlohmann::json &j, const Atom &atom);
void from_json(const nlohmann::json &j, Atom &atom);

} // namespace kakoune

#endif
