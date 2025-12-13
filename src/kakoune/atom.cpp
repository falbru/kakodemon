#include "atom.hpp"

void kakoune::to_json(nlohmann::json &j, const Atom &a)
{
    j = nlohmann::json{{"face", a.face}, {"contents", a.contents.toString()}};
}

void kakoune::from_json(const nlohmann::json &j, Atom &a)
{
    j.at("face").get_to(a.face);
    std::string tmp;
    j.at("contents").get_to(tmp);
    a.contents = domain::UTF8String(tmp);
}
