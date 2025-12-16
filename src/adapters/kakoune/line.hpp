#ifndef KAKOUNE_LINE_HPP_INCLUDED
#define KAKOUNE_LINE_HPP_INCLUDED

#include <vector>

#include "atom.hpp"
#include "domain/line.hpp"
#include "nlohmann/json.hpp"

namespace kakoune
{

struct Line
{
    std::vector<Atom> atoms;

    int size() const;
    domain::UTF8String toUTF8String() const;
};

domain::Line toDomain(Line line);
std::vector<domain::Line> toDomain(std::vector<Line> line);

void to_json(nlohmann::json &j, const Line &line);
void from_json(const nlohmann::json &j, Line &line);

} // namespace kakoune

#endif
