#include "line.hpp"
#include "domain/atom.hpp"
#include "atom.hpp"

void kakoune::to_json(nlohmann::json &j, const Line &l)
{
    j = l.atoms;
}

void kakoune::from_json(const nlohmann::json &j, Line &l)
{
    l.atoms = j.get<std::vector<Atom>>();
}

int kakoune::Line::size() const {
    return atoms.size();
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

domain::Line kakoune::toDomain(Line line) {
    std::vector<domain::Atom> domain_atoms;
    domain_atoms.reserve(line.size());

    for (int i = 0; i < line.size(); i++) {
        domain_atoms.push_back(toDomain(line.atoms[i]));
    }

    return domain::Line{
        domain_atoms
    };

}

std::vector<domain::Line> kakoune::toDomain(std::vector<Line> lines) {
    std::vector<domain::Line> domain_lines;
    domain_lines.reserve(lines.size());

    for (int i = 0; i < lines.size(); i++) {
        domain_lines.push_back(toDomain(lines[i]));
    }

    return domain_lines;
}
