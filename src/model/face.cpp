#include "face.hpp"

void to_json(nlohmann::json &j, const Face &f)
{
    j = nlohmann::json{{"fg", f.fg}, {"bg", f.bg}};
}

void from_json(const nlohmann::json &j, Face &f)
{
    j.at("fg").get_to(f.fg);
    j.at("bg").get_to(f.bg);
}
