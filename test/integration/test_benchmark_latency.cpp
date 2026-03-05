#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <unistd.h>

#include "adapters/kakoune/jsonrpckakouneinterface.hpp"
#include "adapters/kakoune/kakouneclientprocess.hpp"
#include "adapters/kakoune/localsession.hpp"

void waitForRefresh(std::mutex &mtx, std::condition_variable &cv, bool &refreshed)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait_for(lock, std::chrono::seconds(5), [&] { return refreshed; });
    refreshed = false;
}

TEST_CASE("Benchmark: JsonRpcKakouneInterface request-response latency", "[benchmark][integration]")
{
    std::string session_id = "bench_jsonrpc_" + std::to_string(getpid());
    auto local_session = std::make_unique<LocalSession>(session_id);
    local_session->start({}, true);

    auto interface = std::make_unique<kakoune::JsonRpcKakouneInterface>(*local_session, std::nullopt);

    std::mutex mtx;
    std::condition_variable cv;
    bool refreshed = false;

    interface->onRefresh([&](bool) {
        std::lock_guard<std::mutex> lock(mtx);
        refreshed = true;
        cv.notify_one();
    });

    waitForRefresh(mtx, cv, refreshed);

    BENCHMARK("pressKeys to onRefresh") {
        {
            std::lock_guard<std::mutex> lock(mtx);
            refreshed = false;
        }
        interface->pressKeys({"a"});
        waitForRefresh(mtx, cv, refreshed);
    };

    BENCHMARK("10 pressKeys sent before collecting response") {
        {
            std::lock_guard<std::mutex> lock(mtx);
            refreshed = false;
        }

        for (int i = 0; i < 10; ++i)
            interface->pressKeys({"a"});

        waitForRefresh(mtx, cv, refreshed);
    };
}

TEST_CASE("Benchmark: KakouneClientProcess request callback latency", "[benchmark][integration]")
{
    std::string session_id = "bench_clientproc_" + std::to_string(getpid());
    auto local_session = std::make_unique<LocalSession>(session_id);
    local_session->start({}, true);

    auto process = std::make_unique<KakouneClientProcess>(session_id);

    std::mutex mtx;
    std::condition_variable cv;
    bool refreshed = false;

    process->setRequestCallback([&](const IncomingRequest &request) {
        if (request.type == IncomingRequestType::REFRESH) {
            std::lock_guard<std::mutex> lock(mtx);
            refreshed = true;
            cv.notify_one();
        }
    });

    process->start();

    std::atomic<bool> polling{true};
    std::thread polling_thread([&] {
        while (polling)
            process->pollForRequests();
    });

    waitForRefresh(mtx, cv, refreshed);

    BENCHMARK("sendRequest to REFRESH callback") {
        process->sendRequest(OutgoingRequest{OutgoingRequestType::KEYS, KeysRequestData{{"a"}}});
        waitForRefresh(mtx, cv, refreshed);
    };

    BENCHMARK("10 sendRequests sent before collecting responses") {
        {
            std::lock_guard<std::mutex> lock(mtx);
            refreshed = false;
        }

        for (int i = 0; i < 10; ++i) {
            process->sendRequest(OutgoingRequest{OutgoingRequestType::KEYS, KeysRequestData{{"a"}}});
        }

        waitForRefresh(mtx, cv, refreshed);
    };

    polling = false;
    polling_thread.join();
}
