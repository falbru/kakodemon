#include "cliparser.hpp"
#include <getopt.h>
#include <random>
#include <sstream>
#include <iomanip>

std::string generate_random_session_id()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 0xFFFFFF);

    std::stringstream ss;
    ss << "kakod-" << std::hex << std::setfill('0') << std::setw(6) << dis(gen);
    return ss.str();
}

ParsedCliArgs parseCliArgs(int argc, char* argv[])
{
    ParsedCliArgs result;
    result.result = ParseResult::Success;

    bool remote_session_set = false;
    bool local_session_set = false;

    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    optind = 1;

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "c:s:e:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'c':
                result.config.session_type = SessionType::Remote;
                result.config.session_id = optarg;
                remote_session_set = true;
                break;
            case 's':
                result.config.session_id = optarg;
                local_session_set = true;
                break;
            case 'e':
                result.config.startup_command = optarg;
                break;
            case 'v':
                result.result = ParseResult::ShowVersion;
                return result;
            case 'h':
                result.result = ParseResult::ShowHelp;
                return result;
            default:
                result.result = ParseResult::Error;
                result.error_message = "Invalid option";
                return result;
        }
    }

    if (remote_session_set && local_session_set)
    {
        result.result = ParseResult::Error;
        result.error_message = "Error: -c and -s options are not compatible";
        return result;
    }

    if (result.config.session_id.empty())
    {
        result.config.session_id = generate_random_session_id();
    }

    return result;
}
