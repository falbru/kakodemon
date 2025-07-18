#ifndef KAKOUNECLIENTPROCESS_HPP_INCLUDED
#define KAKOUNECLIENTPROCESS_HPP_INCLUDED

#include <queue>
#include <sys/poll.h>

#include <string>
#include <variant>
#include <vector>

#include "line.hpp"

enum class KakouneRequestType
{
    DRAW,
    DRAW_STATUS,
    MENU_SHOW,
    MENU_SELECT,
    MENU_HIDE,
    INFO_SHOW,
    INFO_HIDE,
    SET_CURSOR,
    SET_UI_OPTIONS,
    REFRESH
};

struct KakouneDrawRequestData
{
    std::vector<Line> lines;
};

struct KakouneRefreshRequestData
{
    bool force;
};

using KakouneRequestData = std::variant<KakouneDrawRequestData, KakouneRefreshRequestData>;

struct KakouneClientRequest
{
    KakouneRequestType type;
    KakouneRequestData data;
};

class KakouneClientProcess
{
  public:
    KakouneClientProcess(const std::string &session_name);
    ~KakouneClientProcess();

    void start();
    void pollForRequests();
    std::optional<KakouneClientRequest> getNextRequest();

  private:
    std::string m_session_name;

    int m_pipefd[2];
    pollfd m_pollfd;
    char m_buffer[8192];

    std::queue<std::string> m_request_queue;
    std::string m_request_remainder;
};

#endif
