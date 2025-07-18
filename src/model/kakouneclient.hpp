#ifndef KAKOUNECLIENT_HPP_INCLUDED
#define KAKOUNECLIENT_HPP_INCLUDED

#include "kakouneclientprocess.hpp"
#include "line.hpp"

struct KakouneClient
{
  public:
    KakouneClient(const std::string &session_name);
    ~KakouneClient();

    void init();

    void pollForRequests();
    std::optional<KakouneClientRequest> getNextRequest();

    void setWindowContent(const std::vector<Line> &line);
    const std::vector<Line> &getWindowContent() const;

  private:
    std::vector<Line> m_window_content;
    std::unique_ptr<KakouneClientProcess> m_process;
};

#endif
