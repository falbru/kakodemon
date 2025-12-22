#ifndef KAKOUNE_JSONRPCKAKOUNEINTERFACE_HPP_INCLUDED
#define KAKOUNE_JSONRPCKAKOUNEINTERFACE_HPP_INCLUDED

#include "domain/mouse.hpp"
#include "domain/ports/kakouneinterface.hpp"
#include "kakouneclientprocess.hpp"
#include "kakouneframestatemanager.hpp"

namespace kakoune
{

class JsonRpcKakouneInterface : public domain::KakouneInterface
{
  public:
    JsonRpcKakouneInterface(const domain::KakouneSession &session);
    ~JsonRpcKakouneInterface();

    std::optional<domain::KakouneState> getNextKakouneState();

    void pressKeys(const std::vector<std::string> &keys) override;
    void resize(int rows, int columns) override;
    void scroll(int amount, int line, int column) override;
    void moveMouse(int line, int column) override;
    void pressMouseButton(domain::MouseButton button, int line, int column) override;
    void releaseMouseButton(domain::MouseButton button, int line, int column) override;
    void selectMenuItem(int index) override;

  private:
    std::unique_ptr<KakouneClientProcess> m_process;
    std::unique_ptr<KakouneFrameStateManager> m_frame_state_manager;

    std::string getMouseButtonString(domain::MouseButton button);
};

} // namespace kakoune

#endif
