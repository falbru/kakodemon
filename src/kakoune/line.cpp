#include "line.hpp"
#include "kakoune/atom.hpp"

void kakoune::to_json(nlohmann::json &j, const Line &l)
{
    j = l.atoms;
}

void kakoune::from_json(const nlohmann::json &j, Line &l)
{
    l.atoms = j.get<std::vector<Atom>>();
}

int kakoune::Line::size() const {
    int size = 0;
    for (int i = 0; i < atoms.size(); i++) {
        size += atoms[i].contents.size();
    }
    return size;
}

domain::UTF8String kakoune::Line::toUTF8String() const {
    domain::UTF8String string;
    for (int i = 0; i < atoms.size(); i++) {
        for (int j = 0; j < atoms[i].contents.size(); j++) {
            string.addCodepoint(atoms[i].contents.at(j));
        }
    }
    return string;
}
