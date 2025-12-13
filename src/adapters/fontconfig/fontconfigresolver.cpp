#include "fontconfigresolver.hpp"
#include "domain/ports/fontresolver.hpp"

FontconfigResolver::FontconfigResolver() {

}

FontconfigResolver::~FontconfigResolver() {

}

FontMatch FontconfigResolver::resolve(std::string pattern) {
    return FontMatch();
}
