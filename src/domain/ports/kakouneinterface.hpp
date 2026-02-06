#ifndef DOMAIN_KAKOUNEINTERFACE_HPP_INCLUDED
#define DOMAIN_KAKOUNEINTERFACE_HPP_INCLUDED

#include "domain/editor.hpp"
#include "domain/fontmanager.hpp"
#include "domain/infobox.hpp"
#include "domain/lines.hpp"
#include "domain/menu.hpp"
#include "domain/modeline.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/kakounesession.hpp"
#include "domain/uioptions.hpp"

#include <string>
#include <vector>

using ObserverId = int;

namespace domain
{

struct KakouneState
{
    Lines content;
    CursorPosition cursor_position;
    ModeLine mode_line;
    std::optional<InfoBox> info_box;
    std::optional<Menu> menu;
    Face default_face;
};

struct FrameEvents
{
    bool menu_select = false;
    bool ui_options_updated = false;
};

class KakouneInterface
{
  public:
    virtual ~KakouneInterface() = default;

    virtual std::optional<KakouneState> getNextKakouneState() = 0;
    virtual UIOptions getUIOptions(FontManager *font_manager) = 0;
    virtual FrameEvents getEvents() = 0;
    virtual std::optional<std::pair<KakouneState, FrameEvents>> getNextKakouneStateAndEvents() = 0;

    virtual void pressKeys(const std::vector<std::string> &keys) = 0;
    virtual void resize(int rows, int columns) = 0;
    virtual void scroll(int amount, int line, int column) = 0;
    virtual void moveMouse(int line, int column) = 0;
    virtual void pressMouseButton(MouseButton button, int line, int column) = 0;
    virtual void releaseMouseButton(MouseButton button, int line, int column) = 0;
    virtual void selectMenuItem(int index) = 0;

    virtual ObserverId onRefresh(const std::function<void(bool)> &callback) = 0;
    virtual ObserverId onExit(const std::function<void()> &callback) = 0;
    virtual void removeObserver(ObserverId id) = 0;

  protected:
    KakouneInterface(const KakouneSession &session);

    std::string m_session_id;
};

} // namespace domain

#endif
