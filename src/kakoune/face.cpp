#include "face.hpp"

void kakoune::to_json(nlohmann::json &j, const Face &f)
{
    j = nlohmann::json{{"fg", f.fg}, {"bg", f.bg}};
}

void kakoune::from_json(const nlohmann::json &j, Face &f)
{
    j.at("fg").get_to(f.fg);
    j.at("bg").get_to(f.bg);
}
