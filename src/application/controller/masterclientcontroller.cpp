#include "masterclientcontroller.hpp"
#include "application/model/kakouneclient.hpp"
#include <algorithm>
#include <sstream>

void MasterClientController::init(domain::KakouneSession* session, PaneLayout* pane_layout)
{
    m_session = session;
    m_pane_layout = pane_layout;

    m_arrange_observer = m_pane_layout->onArrange([this](const std::vector<Pane>& panes) {
        updateMasterClients();
    });
}

MasterClientController::~MasterClientController()
{
    if (m_pane_layout) {
        m_pane_layout->removeObserver(m_arrange_observer);
    }
}

std::vector<std::string> MasterClientController::getMasterClientNames() const
{
    std::vector<std::string> master_names;
    const auto& panes = m_pane_layout->getPanes();
    int num_masters = m_pane_layout->getNumMasters();

    for (int i = 0; i < std::min(num_masters, static_cast<int>(panes.size())); ++i)
    {
        if (panes[i].client && !panes[i].client->client_name.empty())
        {
            master_names.push_back(panes[i].client->client_name);
        }
    }

    return master_names;
}

void MasterClientController::updateMasterClients()
{
    auto master_names = getMasterClientNames();

    if (master_names == m_cached_master_names)
    {
        return;
    }

    m_cached_master_names = master_names;

    std::ostringstream command_stream;
    command_stream << "set-option global kakodemon_masters ";

    for (size_t i = 0; i < master_names.size(); ++i)
    {
        if (i > 0) command_stream << " ";
        command_stream << master_names[i];
    }

    std::string command = command_stream.str();
    m_session->sendCommand(command);
}
