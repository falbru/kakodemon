#include <catch2/catch_test_macros.hpp>
#include "application/cliparser.hpp"
#include <vector>

struct ArgvHelper
{
    std::vector<char*> ptrs;
    std::vector<std::string> storage;

    ArgvHelper(const std::vector<std::string>& args)
    {
        storage = args;
        for (auto& arg : storage)
        {
            ptrs.push_back(&arg[0]);
        }
        ptrs.push_back(nullptr);
    }

    char** argv() { return ptrs.data(); }
    int argc() { return static_cast<int>(storage.size()); }
};

TEST_CASE("CLI Parser - No arguments creates local session with random ID", "[cliparser]")
{
    ArgvHelper args({"kakod"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Local);
    REQUIRE(result.config.session_id.find("kakod-") == 0);
    REQUIRE(!result.config.startup_command.has_value());
}

TEST_CASE("CLI Parser - Set session name with -s", "[cliparser]")
{
    ArgvHelper args({"kakod", "-s", "my-session"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Local);
    REQUIRE(result.config.session_id == "my-session");
    REQUIRE(!result.config.startup_command.has_value());
}

TEST_CASE("CLI Parser - Connect to remote session with -c", "[cliparser]")
{
    ArgvHelper args({"kakod", "-c", "remote-session"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Remote);
    REQUIRE(result.config.session_id == "remote-session");
    REQUIRE(!result.config.startup_command.has_value());
}

TEST_CASE("CLI Parser - Set startup command with -e", "[cliparser]")
{
    ArgvHelper args({"kakod", "-e", "echo hello"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.startup_command.has_value());
    REQUIRE(result.config.startup_command.value() == "echo hello");
}

TEST_CASE("CLI Parser - Startup command with local session", "[cliparser]")
{
    ArgvHelper args({"kakod", "-s", "my-session", "-e", "edit file.txt"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Local);
    REQUIRE(result.config.session_id == "my-session");
    REQUIRE(result.config.startup_command.has_value());
    REQUIRE(result.config.startup_command.value() == "edit file.txt");
}

TEST_CASE("CLI Parser - Startup command with remote session", "[cliparser]")
{
    ArgvHelper args({"kakod", "-c", "remote", "-e", "buffer next"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Remote);
    REQUIRE(result.config.session_id == "remote");
    REQUIRE(result.config.startup_command.has_value());
    REQUIRE(result.config.startup_command.value() == "buffer next");
}

TEST_CASE("CLI Parser - Version flag", "[cliparser]")
{
    ArgvHelper args({"kakod", "--version"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::ShowVersion);
}

TEST_CASE("CLI Parser - Help flag", "[cliparser]")
{
    ArgvHelper args({"kakod", "--help"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::ShowHelp);
}

TEST_CASE("CLI Parser - Conflicting -c and -s flags", "[cliparser]")
{
    ArgvHelper args({"kakod", "-c", "remote", "-s", "local"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Error);
    REQUIRE(result.error_message.find("not compatible") != std::string::npos);
}

TEST_CASE("CLI Parser - Random session ID is unique", "[cliparser]")
{
    std::string id1 = generate_random_session_id();
    std::string id2 = generate_random_session_id();
    std::string id3 = generate_random_session_id();

    REQUIRE(id1.find("kakod-") == 0);
    REQUIRE(id2.find("kakod-") == 0);
    REQUIRE(id3.find("kakod-") == 0);

    REQUIRE(id1 != id2);
    REQUIRE(id2 != id3);
    REQUIRE(id1 != id3);
}

TEST_CASE("CLI Parser - Multiple options in different order", "[cliparser]")
{
    ArgvHelper args({"kakod", "-e", "nop", "-s", "test-session"});
    ParsedCliArgs result = parseCliArgs(args.argc(), args.argv());

    REQUIRE(result.result == ParseResult::Success);
    REQUIRE(result.config.session_type == SessionType::Local);
    REQUIRE(result.config.session_id == "test-session");
    REQUIRE(result.config.startup_command.value() == "nop");
}
