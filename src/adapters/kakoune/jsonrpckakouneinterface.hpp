#ifndef KAKOUNE_JSONRPCKAKOUNEINTERFACE_HPP_INCLUDED
#define KAKOUNE_JSONRPCKAKOUNEINTERFACE_HPP_INCLUDED

#include "adapters/kakoune/uioptions.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include "kakouneclientprocess.hpp"
#include "kakouneframestatemanager.hpp"

namespace kakoune
{

class JsonRpcKakouneInterface : public domain::KakouneInterface
{
  public:
    JsonRpcKakouneInterface(const domain::KakouneSession &session, std::optional<std::string> startup_command,
                            const std::vector<std::string> &file_arguments = {});
    ~JsonRpcKakouneInterface();

    std::optional<domain::KakouneState> getNextKakouneState() override;
    domain::UIOptions getUIOptions(domain::FontManager *font_manager) override;
    domain::FrameEvents getEvents() override;
    std::optional<std::pair<domain::KakouneState, domain::FrameEvents>> getNextKakouneStateAndEvents() override;

    void pressKeys(const std::vector<std::string> &keys) override;
    void resize(int rows, int columns) override;
    void scroll(int amount, int line, int column) override;
    void moveMouse(int line, int column) override;
    void pressMouseButton(domain::MouseButton button, int line, int column) override;
    void releaseMouseButton(domain::MouseButton button, int line, int column) override;
    void selectMenuItem(int index) override;

    ObserverId onRefresh(const std::function<void(bool)> &callback) override;
    ObserverId onExit(const std::function<void()> &callback) override;
    void removeObserver(ObserverId id) override;

  private:
    std::unique_ptr<KakouneClientProcess> m_process;
    std::unique_ptr<KakouneFrameStateManager> m_frame_state_manager;
    UIOptions m_ui_options;

    std::string getMouseButtonString(domain::MouseButton button);
    domain::KakouneState convertFrameStateToKakouneState(const FrameState &frame_state);
};

} // namespace kakoune

#endif
