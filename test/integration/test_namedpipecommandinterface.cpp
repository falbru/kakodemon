#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <chrono>
#include <atomic>
#include <unistd.h>

#include "adapters/namedpipe/namedpipecommandinterface.hpp"

TEST_CASE("NamedPipeCommandInterface receives commands", "[integration][namedpipe]") {
    std::string pipe_id = "test_pipe_" + std::to_string(getpid());

    auto receiver = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Receive);
    receiver->init();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto sender = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Send);

    SECTION("receives single command without args") {
        Command cmd;
        cmd.name = "test-command";

        REQUIRE(sender->sendCommand(cmd));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto commands = receiver->getPendingCommands();
        REQUIRE(commands.size() == 1);
        REQUIRE(commands[0].name == "test-command");
        REQUIRE(commands[0].args.empty());
    }

    SECTION("receives command with args") {
        Command cmd;
        cmd.name = "test-command";
        cmd.args = {"arg1", "arg2", "arg3"};

        REQUIRE(sender->sendCommand(cmd));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto commands = receiver->getPendingCommands();
        REQUIRE(commands.size() == 1);
        REQUIRE(commands[0].name == "test-command");
        REQUIRE(commands[0].args.size() == 3);
        REQUIRE(commands[0].args[0] == "arg1");
        REQUIRE(commands[0].args[1] == "arg2");
        REQUIRE(commands[0].args[2] == "arg3");
    }

    SECTION("receives multiple commands") {
        Command cmd1;
        cmd1.name = "first-command";

        Command cmd2;
        cmd2.name = "second-command";
        cmd2.args = {"value"};

        REQUIRE(sender->sendCommand(cmd1));
        REQUIRE(sender->sendCommand(cmd2));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto commands = receiver->getPendingCommands();
        REQUIRE(commands.size() == 2);
        REQUIRE(commands[0].name == "first-command");
        REQUIRE(commands[1].name == "second-command");
        REQUIRE(commands[1].args.size() == 1);
    }

    SECTION("getPendingCommands clears the queue") {
        Command cmd;
        cmd.name = "test-command";

        REQUIRE(sender->sendCommand(cmd));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto commands_first = receiver->getPendingCommands();
        REQUIRE(commands_first.size() == 1);

        auto commands_second = receiver->getPendingCommands();
        REQUIRE(commands_second.empty());
    }
}

TEST_CASE("NamedPipeCommandInterface wake callback is invoked", "[integration][namedpipe]") {
    std::string pipe_id = "test_pipe_callback_" + std::to_string(getpid());

    auto receiver = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Receive);

    std::atomic<int> callback_count{0};
    receiver->setWakeEventLoopCallback([&callback_count]() {
        callback_count++;
    });

    receiver->init();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto sender = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Send);

    Command cmd;
    cmd.name = "test-command";

    REQUIRE(sender->sendCommand(cmd));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    REQUIRE(callback_count.load() >= 1);
}

TEST_CASE("NamedPipeCommandInterface mode restrictions", "[integration][namedpipe]") {
    std::string pipe_id = "test_pipe_modes_" + std::to_string(getpid());

    SECTION("Send mode cannot receive commands") {
        auto sender = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Send);
        auto commands = sender->getPendingCommands();
        REQUIRE(commands.empty());
    }

    SECTION("Receive mode cannot send commands") {
        auto receiver = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Receive);
        receiver->init();

        Command cmd;
        cmd.name = "test-command";

        REQUIRE_FALSE(receiver->sendCommand(cmd));
    }
}

TEST_CASE("NamedPipeCommandInterface send to non-existent pipe fails", "[integration][namedpipe]") {
    std::string pipe_id = "nonexistent_pipe_" + std::to_string(getpid());

    auto sender = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Send);

    Command cmd;
    cmd.name = "test-command";

    REQUIRE_FALSE(sender->sendCommand(cmd));
}

TEST_CASE("NamedPipeCommandInterface pipe cleanup on destruction", "[integration][namedpipe]") {
    std::string pipe_id = "test_pipe_cleanup_" + std::to_string(getpid());
    std::string pipe_path = "/tmp/kakod-" + pipe_id;

    {
        auto receiver = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Receive);
        receiver->init();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        REQUIRE(access(pipe_path.c_str(), F_OK) == 0);
    }

    REQUIRE(access(pipe_path.c_str(), F_OK) != 0);
}

TEST_CASE("NamedPipeCommandInterface ping command round-trip", "[integration][namedpipe]") {
    std::string pipe_id = "test_pipe_ping_" + std::to_string(getpid());

    auto receiver = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Receive);
    receiver->init();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto sender = std::make_unique<NamedPipeCommandInterface>(pipe_id, PipeMode::Send);

    Command ping_cmd;
    ping_cmd.name = "ping";

    REQUIRE(sender->sendCommand(ping_cmd));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto commands = receiver->getPendingCommands();
    REQUIRE(commands.size() == 1);
    REQUIRE(commands[0].name == "ping");
}

TEST_CASE("NamedPipeCommandInterface Both mode can send and receive", "[integration][namedpipe]") {
    std::string pipe_id_a = "test_pipe_both_a_" + std::to_string(getpid());
    std::string pipe_id_b = "test_pipe_both_b_" + std::to_string(getpid());

    auto interface_a = std::make_unique<NamedPipeCommandInterface>(pipe_id_a, PipeMode::Both);
    auto interface_b = std::make_unique<NamedPipeCommandInterface>(pipe_id_b, PipeMode::Both);

    interface_a->init();
    interface_b->init();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto sender_to_a = std::make_unique<NamedPipeCommandInterface>(pipe_id_a, PipeMode::Send);
    auto sender_to_b = std::make_unique<NamedPipeCommandInterface>(pipe_id_b, PipeMode::Send);

    Command cmd_to_a;
    cmd_to_a.name = "message-to-a";

    Command cmd_to_b;
    cmd_to_b.name = "message-to-b";

    REQUIRE(sender_to_a->sendCommand(cmd_to_a));
    REQUIRE(sender_to_b->sendCommand(cmd_to_b));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto commands_a = interface_a->getPendingCommands();
    auto commands_b = interface_b->getPendingCommands();

    REQUIRE(commands_a.size() == 1);
    REQUIRE(commands_a[0].name == "message-to-a");

    REQUIRE(commands_b.size() == 1);
    REQUIRE(commands_b[0].name == "message-to-b");
}
