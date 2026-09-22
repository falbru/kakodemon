#ifndef ATOM_HPP_INCLUDED
#define ATOM_HPP_INCLUDED

#include "domain/atom.hpp"
#include "domain/codepointstring.hpp"
#include "face.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Atom
{
    Face face;
    domain::CodepointString contents;
};

void to_json(nlohmann::json &j, const Atom &atom);
void from_json(const nlohmann::json &j, Atom &atom);

domain::Atom toDomain(kakoune::Atom atom);

} // namespace kakoune

#endif
