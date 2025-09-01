#include "coord.hpp"

void kakoune::to_json(nlohmann::json &j, const Coord &coord)
{
    j = nlohmann::json{{"line", coord.line}, {"column", coord.column}};
}

void kakoune::from_json(const nlohmann::json &j, Coord &coord)
{
    j.at("line").get_to(coord.line);
    j.at("column").get_to(coord.column);
}
