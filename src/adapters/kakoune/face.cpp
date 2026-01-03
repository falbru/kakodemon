#include "face.hpp"

void kakoune::to_json(nlohmann::json &j, const Attribute &a)
{
    switch (a) {
        case UNDERLINE:        j = "underline"; break;
        case CURLY_UNDERLINE:  j = "curly_underline"; break;
        case DOUBLE_UNDERLINE: j = "double_underline"; break;
        case REVERSE:          j = "reverse"; break;
        case BLINK:            j = "blink"; break;
        case BOLD:             j = "bold"; break;
        case DIM:              j = "dim"; break;
        case ITALIC:           j = "italic"; break;
        case FINAL_FG:         j = "final_fg"; break;
        case FINAL_BG:         j = "final_bg"; break;
        case FINAL_ATTR:       j = "final_attr"; break;
    }
}

void kakoune::from_json(const nlohmann::json &j, Attribute &a)
{
    std::string attr_str = j.get<std::string>();

    if (attr_str == "underline")             a = UNDERLINE;
    else if (attr_str == "curly_underline")  a = CURLY_UNDERLINE;
    else if (attr_str == "double_underline") a = DOUBLE_UNDERLINE;
    else if (attr_str == "reverse")          a = REVERSE;
    else if (attr_str == "blink")            a = BLINK;
    else if (attr_str == "bold")             a = BOLD;
    else if (attr_str == "dim")              a = DIM;
    else if (attr_str == "italic")           a = ITALIC;
    else if (attr_str == "final_fg")         a = FINAL_FG;
    else if (attr_str == "final_bg")         a = FINAL_BG;
    else if (attr_str == "final_attr")       a = FINAL_ATTR;
    else {
        throw nlohmann::json::parse_error::create(
            101, 0, "Invalid attribute value: " + attr_str, nullptr);
    }
}

void kakoune::to_json(nlohmann::json &j, const Face &f)
{
    j = nlohmann::json{{"fg", f.fg}, {"bg", f.bg}, {"attributes", f.attributes}};
}

void kakoune::from_json(const nlohmann::json &j, Face &f)
{
    j.at("fg").get_to(f.fg);
    j.at("bg").get_to(f.bg);
    j.at("attributes").get_to(f.attributes);
}

domain::Attribute toDomain(kakoune::Attribute attr) {
    switch (attr) {
        case kakoune::UNDERLINE:        return domain::Attribute::Underline;
        case kakoune::CURLY_UNDERLINE:  return domain::Attribute::CurlyUnderline;
        case kakoune::DOUBLE_UNDERLINE: return domain::Attribute::DoubleUnderline;
        case kakoune::REVERSE:          return domain::Attribute::Reverse;
        case kakoune::BLINK:            return domain::Attribute::Blink;
        case kakoune::BOLD:             return domain::Attribute::Bold;
        case kakoune::DIM:              return domain::Attribute::Dim;
        case kakoune::ITALIC:           return domain::Attribute::Italic;
        case kakoune::FINAL_FG:         return domain::Attribute::FinalFg;
        case kakoune::FINAL_BG:         return domain::Attribute::FinalBg;
        case kakoune::FINAL_ATTR:       return domain::Attribute::FinalAttr;
    }
}

domain::Face kakoune::toDomain(kakoune::Face face) {
    std::vector<domain::Attribute> attributes;
    attributes.reserve(face.attributes.size());
    for (auto attr : face.attributes) {
        attributes.push_back(::toDomain(attr));
    }
    return domain::Face(toDomain(face.bg), toDomain(face.fg), std::move(attributes));
}
