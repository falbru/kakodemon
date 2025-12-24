#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>

#include "adapters/kakoune/localsession.hpp"
#include "adapters/kakoune/remotesession.hpp"
#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "application/model/kakouneclient.hpp"

TEST_CASE("LocalSession starts a Kakoune session", "[integration][localsession]") {
    std::string session_id = "test_session_" + std::to_string(getpid());

    auto session = std::make_unique<LocalSession>(session_id);

    REQUIRE_NOTHROW(session->start());

    SECTION("returns correct session id") {
        REQUIRE(session->getSessionId() == session_id);
    }

    std::string cmd = "kak -l | grep " + session_id;
    int result = system(cmd.c_str());
    REQUIRE(WEXITSTATUS(result) == 0);
}

TEST_CASE("KakouneClient can connect immediately after LocalSession starts", "[integration][localsession][kakouneclient]") {
    std::string session_id = "test_client_session_" + std::to_string(getpid());

    SECTION("KakouneClient initializes successfully with LocalSession") {
        auto session = std::make_unique<LocalSession>(session_id);
        session->start();

        std::string cmd = "kak -l | grep " + session_id;
        int result = system(cmd.c_str());
        REQUIRE(WEXITSTATUS(result) == 0);

        auto remote_session = std::make_unique<RemoteSession>(session_id);
        auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*remote_session, std::nullopt);

        REQUIRE_NOTHROW([&]() {
            auto client = std::make_unique<KakouneClient>(std::move(remote_session), std::move(interface));
        }());
    }
}

TEST_CASE("LocalSession destructor kills the Kakoune session", "[integration][localsession]") {
    std::string session_id = "test_cleanup_session_" + std::to_string(getpid());

    {
        auto session = std::make_unique<LocalSession>(session_id);
        session->start();

        std::string cmd = "kak -l | grep " + session_id;
        int result = system(cmd.c_str());
        REQUIRE(WEXITSTATUS(result) == 0);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::string cmd = "kak -l | grep " + session_id;
    int result = system(cmd.c_str());
    REQUIRE(WEXITSTATUS(result) != 0);
}

TEST_CASE("Multiple LocalSessions can coexist", "[integration][localsession]") {
    std::string session_id1 = "test_multi_session1_" + std::to_string(getpid());
    std::string session_id2 = "test_multi_session2_" + std::to_string(getpid());

    auto session1 = std::make_unique<LocalSession>(session_id1);
    auto session2 = std::make_unique<LocalSession>(session_id2);

    session1->start();
    session2->start();

    std::string cmd1 = "kak -l | grep " + session_id1;
    std::string cmd2 = "kak -l | grep " + session_id2;

    int result1 = system(cmd1.c_str());
    int result2 = system(cmd2.c_str());

    REQUIRE(WEXITSTATUS(result1) == 0);
    REQUIRE(WEXITSTATUS(result2) == 0);
}

TEST_CASE("RemoteSession connects to existing session", "[integration][remotesession]") {
    std::string session_id = "test_remote_session_" + std::to_string(getpid());

    auto local_session = std::make_unique<LocalSession>(session_id);
    local_session->start();

    auto remote_session = std::make_unique<RemoteSession>(session_id);

    SECTION("returns corrects session_id") {
        REQUIRE(remote_session->getSessionId() == session_id);
    }

    REQUIRE_NOTHROW([&]() {
        auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*remote_session, std::nullopt);
    }());
}
