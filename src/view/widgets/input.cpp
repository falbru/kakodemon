#include "view/widgets/input.hpp"
#include "model/kakouneclient.hpp"

Input::Input(std::shared_ptr<opengl::Font> font)
    : m_font(font) {}

void Input::setContent(const kakoune::Line& content) { m_content = content; }

void Input::render(std::shared_ptr<opengl::Renderer> renderer,
                   const KakouneClient& kakoune_client,
                   LayoutManager &layout) const {
  auto input_layout =
      layout.sliceTop(height());

  renderer->renderRect(kakoune_client.menu_selected_face.bg.toCoreColor(
                           kakoune_client.window_default_face.bg, false),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.border(BORDER);

  renderer->renderRect(kakoune_client.status_default_face.bg.toCoreColor(
                           kakoune_client.window_default_face.bg, false),
                       input_layout.current().x,
                       input_layout.current().y,
                       input_layout.current().width,
                       input_layout.current().height);

  input_layout.pad(PADDING);

  renderer->renderLine(*m_font, kakoune_client.status_line,
                       kakoune_client.status_default_face,
                       input_layout.current().x, input_layout.current().y);
}

float Input::width() const {
  return BORDER * 2 + PADDING * 2 +
         m_content.size() * (m_font->getCharacter('A').Advance >> 6);
}

float Input::height() const {
  return BORDER * 2 + PADDING * 2 + m_font->getLineHeight();
}
