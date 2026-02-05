#ifndef KAKOUNECLIENTPROCESS_HPP_INCLUDED
#define KAKOUNECLIENTPROCESS_HPP_INCLUDED

#include <atomic>
#include <functional>
#include <map>
#include <sys/poll.h>
#include <sys/types.h>

#include <string>
#include <variant>
#include <vector>

#include "adapters/kakoune/uioptions.hpp"
#include "coord.hpp"
#include "infostyle.hpp"
#include "line.hpp"
#include "menustyle.hpp"

enum class IncomingRequestType
{
    DRAW,
    DRAW_STATUS,
    MENU_SHOW,
    MENU_SELECT,
    MENU_HIDE,
    INFO_SHOW,
    INFO_HIDE,
    SET_UI_OPTIONS,
    REFRESH
};

struct DrawRequestData
{
    std::vector<kakoune::Line> lines;
    kakoune::Coord cursor_pos;
    kakoune::Face default_face;
};

struct DrawStatusRequestData
{
    kakoune::Line status_line_prompt;
    kakoune::Line status_line_content;
    int cursor_pos;
    kakoune::Line mode_line;
    kakoune::Face default_face;
};

struct MenuShowData
{
    std::vector<kakoune::Line> items;
    kakoune::Coord anchor;
    kakoune::Face selected_face;
    kakoune::Face face;
    kakoune::MenuStyle style;
};

struct MenuHideData
{
};

struct MenuSelectData
{
    int selected;
};

struct InfoShowData
{
    kakoune::Line title;
    std::vector<kakoune::Line> content;
    kakoune::Coord anchor;
    kakoune::Face face;
    kakoune::InfoStyle style;
};

struct InfoHideData
{
};

struct RefreshRequestData
{
    bool force;
};

struct UIOptionsData
{
    kakoune::UIOptions ui_options;
};

using IncomingRequestData = std::variant<DrawRequestData, DrawStatusRequestData, RefreshRequestData, MenuShowData,
                                         MenuHideData, MenuSelectData, InfoShowData, InfoHideData, UIOptionsData>;

struct IncomingRequest
{
    IncomingRequestType type;
    IncomingRequestData data;
};

enum class OutgoingRequestType
{
    KEYS,
    RESIZE,
    MOUSE_MOVE,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MENU_SELECT,
    SCROLL
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

struct MouseMoveRequestData
{
    int line;
    int column;
};

struct MousePressRequestData
{
    std::string button;
    int line;
    int column;
};

struct MouseReleaseRequestData
{
    std::string button;
    int line;
    int column;
};

struct MenuSelectRequestData
{
    int selected_index;
};

struct ScrollRequestData
{
    int amount;
    int line;
    int column;
};

using OutgoingRequestData =
    std::variant<KeysRequestData, ResizeRequestData, MouseMoveRequestData, MousePressRequestData,
                 MouseReleaseRequestData, MenuSelectRequestData, ScrollRequestData>;

struct OutgoingRequest
{
    OutgoingRequestType type;
    OutgoingRequestData data;
};

using ObserverId = int;

class KakouneClientProcess
{
  public:
    KakouneClientProcess(const std::string &session_name);
    ~KakouneClientProcess();

    void start();
    void start(std::optional<std::string> startup_command);
    void start(std::optional<std::string> startup_command, const std::vector<std::string> &file_arguments);

    void pollForRequests();
    void setRequestCallback(const std::function<void(const IncomingRequest &)> &callback);
    ObserverId onExit(const std::function<void()> &callback);
    void removeObserver(ObserverId id);

    void sendRequest(const OutgoingRequest &request);

    static void processPendingExits();

  private:
    static void handleSIGCHLD(int sig);
    static std::map<pid_t, KakouneClientProcess *> pid_to_instances;
    void registerProcess(pid_t pid);
    pid_t m_client_pid;

    ObserverId m_next_observer_id = 0;
    std::map<ObserverId, std::function<void()>> m_exit_observers;
    void notifyExitObservers();

    std::optional<IncomingRequest> parseRequest(std::string request);

    std::string m_session_name;

    int m_stdout_pipefd[2] = {-1, -1};
    int m_stdin_pipefd[2] = {-1, -1};

    pollfd m_pollfd;
    char m_buffer[8192];

    std::function<void(const IncomingRequest &)> m_request_callback;
    std::string m_request_remainder;

    std::atomic<bool> m_exited{false};
};

#endif
