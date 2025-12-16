#ifndef COORD_HPP_INCLUDED
#define COORD_HPP_INCLUDED

#include "domain/coord.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{
struct Coord
{
    int line;
    int column;
};

void to_json(nlohmann::json &j, const Coord &coord);
void from_json(const nlohmann::json &j, Coord &coord);

domain::Coord toDomain(Coord coord);

} // namespace kakoune

#endif // coord_hpp_INCLUDED
