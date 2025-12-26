#ifndef FREETYPELIBRARY_HPP_INCLUDED
#define FREETYPELIBRARY_HPP_INCLUDED

#include <freetype/freetype.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class FreeTypeLibrary
{
  public:
    FreeTypeLibrary();
    ~FreeTypeLibrary();

    FreeTypeLibrary(const FreeTypeLibrary &) = delete;
    FreeTypeLibrary &operator=(const FreeTypeLibrary &) = delete;
    FreeTypeLibrary(FreeTypeLibrary &&) = delete;
    FreeTypeLibrary &operator=(FreeTypeLibrary &&) = delete;

    FT_Library get() const
    {
        return m_library;
    }
    bool isValid() const
    {
        return m_library != nullptr;
    }

  private:
    FT_Library m_library;
};

#endif
