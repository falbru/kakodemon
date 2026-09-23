#ifndef DOMAIN_UTF8STRING_INCLUDED
#define DOMAIN_UTF8STRING_INCLUDED

#include <string>
#include <vector>

namespace domain
{

typedef unsigned int Codepoint;

std::string codePointToString(Codepoint codepoint);
bool isWhitespace(Codepoint codepoint);
bool isControlCharacter(Codepoint codepoint);
bool isEmoji(Codepoint codepoint);

enum class TrimDirection
{
    Left,
    Right,
    Both
};

class CodepointString
{
  public:
    CodepointString();
    CodepointString(std::vector<Codepoint>);
    CodepointString(std::string string);

    std::string toString() const;

    void addCodepoint(Codepoint codepoint);
    void insertCodepoint(int index, Codepoint codepoint);
    void reserve(size_t capacity);

    int size() const;
    Codepoint at(unsigned int index) const;

    std::vector<Codepoint>::iterator begin();
    std::vector<Codepoint>::iterator end();
    std::vector<Codepoint>::const_iterator begin() const;
    std::vector<Codepoint>::const_iterator end() const;

    CodepointString substring(size_t start) const;
    CodepointString substring(size_t start, size_t length) const;
    CodepointString trim(TrimDirection direction = TrimDirection::Both) const;

    CodepointString &operator+=(const CodepointString &other);
    bool operator==(const CodepointString &other) const;
    bool operator!=(const CodepointString &other) const;

  private:
    std::vector<Codepoint> m_codepoints;
};

std::ostream &operator<<(std::ostream &os, const CodepointString &str);
std::string to_string(const CodepointString &str);

} // namespace domain

#endif
