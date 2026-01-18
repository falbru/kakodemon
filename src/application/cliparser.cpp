#include "cliparser.hpp"
#include <cstdlib>
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
    bool send_command = false;

    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    optind = 1;

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "c:s:e:p", long_options, &option_index)) != -1)
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
            case 'p':
                send_command = true;
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

    if (send_command)
    {
        const char* id_env = std::getenv("KAKOD_ID");
        if (!id_env)
        {
            result.result = ParseResult::Error;
            result.error_message = "Error: KAKOD_ID environment variable not set";
            return result;
        }
        result.command_request.pipe_id = id_env;

        if (optind >= argc)
        {
            result.result = ParseResult::Error;
            result.error_message = "Error: -p requires a command";
            return result;
        }

        result.command_request.command = argv[optind++];
        for (int i = optind; i < argc; i++)
        {
            result.command_request.args.push_back(argv[i]);
        }

        result.result = ParseResult::SendCommand;
        return result;
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

    for (int i = optind; i < argc; i++)
    {
        result.config.file_arguments.push_back(argv[i]);
    }

    return result;
}
