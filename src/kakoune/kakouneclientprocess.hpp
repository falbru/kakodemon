#ifndef KAKOUNECLIENTPROCESS_HPP_INCLUDED
#define KAKOUNECLIENTPROCESS_HPP_INCLUDED

#include <queue>
#include <sys/poll.h>

#include <string>
#include <variant>
#include <vector>

#include "line.hpp"

enum class IncomingRequestType
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

struct DrawRequestData
{
    std::vector<Line> lines;
};

struct RefreshRequestData
{
    bool force;
};

using IncomingRequestData = std::variant<DrawRequestData, RefreshRequestData>;

struct IncomingRequest
{
    IncomingRequestType type;
    IncomingRequestData data;
};

enum class OutgoingRequestType
{
    KEYS,
    RESIZE
};

struct KeysRequestData
{
    std::vector<std::string> keys;
};

struct ResizeRequestData
{
    int rows;
    int columns;
};

using OutgoingRequestData = std::variant<KeysRequestData, ResizeRequestData>;

struct OutgoingRequest
{
    OutgoingRequestType type;
    OutgoingRequestData data;
};

class KakouneClientProcess
{
  public:
    KakouneClientProcess(const std::string &session_name);
    ~KakouneClientProcess();

    void start();

    void pollForRequests();
    std::optional<IncomingRequest> getNextRequest();

    void sendRequest(const OutgoingRequest &request);

  private:
    std::optional<IncomingRequest> parseRequest(std::string request);

    std::string m_session_name;

    int m_stdout_pipefd[2];
    int m_stdin_pipefd[2];

    pollfd m_pollfd;
    char m_buffer[8192];

    std::queue<IncomingRequest> m_request_queue;
    std::string m_request_remainder;
};

#endif
