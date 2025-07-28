#include "line.hpp"

void kakoune::to_json(nlohmann::json &j, const Line &l)
{
    j = l.atoms;
}

void kakoune::from_json(const nlohmann::json &j, Line &l)
{
    l.atoms = j.get<std::vector<Atom>>();
}
