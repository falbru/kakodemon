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
