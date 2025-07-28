#include "atom.hpp"

void kakoune::to_json(nlohmann::json &j, const Atom &a)
{
    j = nlohmann::json{{"face", a.face}, {"contents", a.contents}};
}

void kakoune::from_json(const nlohmann::json &j, Atom &a)
{
    j.at("face").get_to(a.face);
    j.at("contents").get_to(a.contents);
}
