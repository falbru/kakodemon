#ifndef LINE_HPP_INCLUDED
#define LINE_HPP_INCLUDED

#include <vector>

#include "atom.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Line
{
    std::vector<Atom> atoms;

    int size() const;
    domain::UTF8String toUTF8String() const;
};

void to_json(nlohmann::json &j, const Line &line);
void from_json(const nlohmann::json &j, Line &line);

} // namespace kakoune

#endif
