#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

#include "adapters/kakoune/kakouneclientprocess.hpp"
#include "adapters/kakoune/localsession.hpp"

TEST_CASE("KakouneClientProcess executes startup_command", "[integration][kakouneclientprocess]") {
    std::string session_id = "test_startup_cmd_" + std::to_string(getpid());

    auto session = std::make_unique<LocalSession>(session_id);
    session->start();

    std::string fifo_path = "/tmp/test_kakodemon_startup_" + std::to_string(getpid()) + ".fifo";

    REQUIRE(mkfifo(fifo_path.c_str(), 0600) == 0);

    std::string startup_command = "echo -to-file " + fifo_path + " test";

    auto client_process = std::make_unique<KakouneClientProcess>(session_id);
    REQUIRE_NOTHROW(client_process->start(startup_command));

    int fd = open(fifo_path.c_str(), O_RDONLY | O_NONBLOCK);
    REQUIRE(fd != -1);

    struct pollfd pfd = {fd, POLLIN, 0};
    int poll_result = poll(&pfd, 1, 5000);
    REQUIRE(poll_result > 0);

    char buffer[256];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink(fifo_path.c_str());

    REQUIRE(bytes_read > 0);
    buffer[bytes_read] = '\0';

    std::string result(buffer);
    REQUIRE(result.find("test") != std::string::npos);
}

TEST_CASE("KakouneClientProcess starts without startup_command", "[integration][kakouneclientprocess]") {
    std::string session_id = "test_no_startup_" + std::to_string(getpid());

    auto session = std::make_unique<LocalSession>(session_id);
    session->start();

    auto client_process = std::make_unique<KakouneClientProcess>(session_id);

    REQUIRE_NOTHROW(client_process->start());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
