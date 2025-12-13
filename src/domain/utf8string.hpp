#ifndef DOMAIN_UTF8STRING_INCLUDED
#define DOMAIN_UTF8STRING_INCLUDED

#include <string>
#include <vector>

namespace domain
{

typedef unsigned int Codepoint;

std::string codePointToString(Codepoint codepoint);

enum class TrimDirection
{
    Left,
    Right,
    Both
};

class UTF8String
{
  public:
    UTF8String();
    UTF8String(std::vector<Codepoint>);
    UTF8String(std::string string);

    std::string toString() const;

    void addCodepoint(Codepoint codepoint);
    void insertCodepoint(int index, Codepoint codepoint);

    int size() const;
    Codepoint at(unsigned int index) const;

    UTF8String substring(size_t start) const;
    UTF8String substring(size_t start, size_t length) const;
    UTF8String trim(TrimDirection direction = TrimDirection::Both) const;

  private:
    std::vector<Codepoint> m_codepoints;
};

} // namespace domain

#endif
