#ifndef DOMAIN_STATUSLINE_HPP_INCLUDED
#define DOMAIN_STATUSLINE_HPP_INCLUDED

#include "domain/line.hpp"
namespace domain
{

class StatusLine
{
  public:
    StatusLine(Line prompt, Line content);

    const Line &getPrompt() const;
    const Line &getContent() const;

  private:
    Line m_prompt;
    Line m_content;
};

} // namespace domain

#endif // statusline_hpp_INCLUDED
