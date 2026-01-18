#include "application/application.hpp"
#include "application/applicationbuilder.hpp"
#include "application/cliparser.hpp"
#include "adapters/namedpipe/namedpipecommandinterface.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>

void print_help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " [OPTIONS] [file]... [+<line>[:<col>]|+:]\n\n"
              << "Options:\n"
              << "  -c <session>      Connect to remote Kakoune session\n"
              << "  -s <session>      Set session name\n"
              << "  -e <command>      Execute command on client initialisation\n"
              << "  -p <command>      Send command to a running instance (uses KAKOD_ID env)\n"
              << "  --version         Print version and exit\n"
              << "  --help            Print this help message and exit\n\n"
              << "File arguments:\n"
              << "  [file]...         Files to edit\n"
              << "  [+<line>[:<col>]] Edit file at specific line (and column)\n"
              << "  [+:]              Edit file at last position\n";
}

void print_version()
{
    std::cout << "kakodemon " << KAKODEMON_VERSION << "\n";
}

int main(int argc, char* argv[])
{
    ParsedCliArgs parsed_args = parseCliArgs(argc, argv);

    switch (parsed_args.result)
    {
        case ParseResult::ShowVersion:
            print_version();
            return 0;
        case ParseResult::ShowHelp:
            print_help(argv[0]);
            return 0;
        case ParseResult::SendCommand:
        {
            NamedPipeCommandInterface interface(parsed_args.command_request.pipe_id, PipeMode::Send);
            Command cmd;
            cmd.name = parsed_args.command_request.command;
            cmd.args = parsed_args.command_request.args;
            if (!interface.sendCommand(cmd)) {
                std::cerr << "Error: Failed to send command\n";
                return 1;
            }
            return 0;
        }
        case ParseResult::Error:
            std::cerr << parsed_args.error_message << "\n";
            return 1;
        case ParseResult::Success:
            break;
    }

    ApplicationBuilder builder;
    std::unique_ptr<Application> app = builder
        .withPlatform(PlatformType::OPENGL_GLFW)
        .withFontResolver(FontResolverType::FONTCONFIG)
        .withFontEngine(FontEngineType::FREETYPE)
        .build();

    app->init(parsed_args.config);
    app->run();
    return 0;
}
