#ifndef CORE_UTF8STRING_INCLUDED
#define CORE_UTF8STRING_INCLUDED

#include <string>
#include <vector>

typedef unsigned int Codepoint;

enum class TrimDirection {
    Left,
    Right,
    Both
};

class UTF8String {
public:
    UTF8String();
    UTF8String(std::string string);

    std::string toString() const;

    void addCodepoint(Codepoint codepoint);
    void insertCodepoint(int index, Codepoint codepoint);

    int size() const;
    Codepoint at(unsigned int index) const;

    UTF8String substring(size_t start) const;
    UTF8String substring(size_t start, size_t length) const;
    void trim(TrimDirection direction = TrimDirection::Both);
private:
    std::vector<Codepoint> m_codepoints;
};

#endif
