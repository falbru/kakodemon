#include "line.hpp"

void to_json(nlohmann::json &j, const Line &l)
{
    j = l.atoms;
}

void from_json(const nlohmann::json &j, Line &l)
{
    l.atoms = j.get<std::vector<Atom>>();
}
