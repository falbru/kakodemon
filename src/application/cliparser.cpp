#include "cliparser.hpp"
#include "application/cliconfig.hpp"
#include <getopt.h>
#include <random>
#include <sstream>
#include <iomanip>

std::string generateRandomSessionId()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 0xFFFFFF);

    std::stringstream ss;
    ss << "kakod-" << std::hex << std::setfill('0') << std::setw(6) << dis(gen);
    return ss.str();
}

CliParser::CliParser(ValidatorDependencies dependencies) : m_validator_depencies(dependencies) { }

ParsedCliArgs CliParser::parseAndValidate(int argc, char* argv[])
{
    ParsedCliArgs result;
    result.result = ParseResult::Success;
    result.config.session_type = SessionType::Local;
    result.config.no_config = false;

    bool send_command = false;

    int set_option_session_type_count = 0;

    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"init", no_argument, 0, 'i'},
        {0, 0, 0, 0}
    };

    optind = 1;

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "c:C:s:e:pn", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'c':
                result.config.session_type = SessionType::Remote;
                result.config.session_id = optarg;
                set_option_session_type_count++;
                break;
            case 'C': {
                    if (m_validator_depencies.kakouneSessionExists(std::string(optarg))) {
                        result.config.session_type = SessionType::Remote;
                        result.config.session_id = optarg;
                    } else {
                        result.config.session_type = SessionType::Local;
                        result.config.session_id = optarg;
                    }
                    set_option_session_type_count++;
                    break;
                }
            case 's':
                result.config.session_id = optarg;
                set_option_session_type_count++;
                break;
            case 'e':
                result.config.startup_command = optarg;
                break;
            case 'p':
                send_command = true;
                break;
            case 'n':
                result.config.no_config = true;
                break;
            case 'v':
                result.result = ParseResult::ShowVersion;
                return result;
            case 'h':
                result.result = ParseResult::ShowHelp;
                return result;
            case 'i':
                result.result = ParseResult::ShowKakodemonIntegrationScript;
                return result;
            default:
                result.result = ParseResult::Error;
                result.error_message = "Invalid option";
                return result;
        }
    }

    if (send_command)
    {
        if (optind >= argc)
        {
            result.result = ParseResult::Error;
            result.error_message = "-p requires a command";
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

    if (set_option_session_type_count > 1)
    {
        result.result = ParseResult::Error;
        result.error_message = "-c, -C and -s options are not compatible";
        return result;
    }

    if (result.config.session_type == SessionType::Remote && result.config.no_config)
    {
        result.result = ParseResult::Error;
        result.error_message = "-n incompatible with connecting to an existing session";
        return result;
    }

    if (result.config.session_id.empty())
    {
        result.config.session_id = generateRandomSessionId();
    }

    if (result.config.session_type == SessionType::Remote && !m_validator_depencies.kakouneSessionExists(result.config.session_id)) {
        result.result = ParseResult::Error;
        result.error_message = "Kakoune session does not exist";
        return result;
    }

    if (result.config.session_type == SessionType::Local && m_validator_depencies.kakouneSessionExists(result.config.session_id)) {
        result.result = ParseResult::Error;
        result.error_message = "Kakoune session name already in use";
        return result;
    }

    for (int i = optind; i < argc; i++)
    {
        result.config.file_arguments.push_back(argv[i]);
    }

    return result;
}
