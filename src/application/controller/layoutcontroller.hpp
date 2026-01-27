#ifndef CONTROLLER_LAYOUTCONTROLLER_HPP_INCLUDED
#define CONTROLLER_LAYOUTCONTROLLER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "domain/geometry.hpp"
#include <memory>
#include <vector>

struct ClientLayout
{
    KakouneClient *client;
    domain::Rectangle bounds;
};

class LayoutController
{
  public:
    void init(std::vector<std::unique_ptr<KakouneClient>> *clients);
    void arrange(float width, float height);
    ClientLayout *findLayoutAt(float x, float y);
    const std::vector<ClientLayout> &getLayouts() const;

  private:
    std::vector<ClientLayout> m_layouts;
};

#endif
