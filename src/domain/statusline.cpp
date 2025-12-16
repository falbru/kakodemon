#include "domain/statusline.hpp"

namespace domain {

StatusLine::StatusLine(Line prompt, Line content) : m_prompt(prompt), m_content(content) {

}

const Line& StatusLine::getPrompt() const {
    return m_prompt;
}

const Line& StatusLine::getContent() const {
    return m_content;
}

}
