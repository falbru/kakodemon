#include "fontconfigresolver.hpp"
#include "domain/ports/fontresolver.hpp"

FontconfigResolver::FontconfigResolver() {

}

FontconfigResolver::~FontconfigResolver() {

}

domain::FontMatch FontconfigResolver::resolve(std::string pattern) {
    return domain::FontMatch();
}
