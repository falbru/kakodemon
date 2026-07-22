#include "application/application.hpp"
#include "application/applicationbuilder.hpp"
#include "application/applicationconfig.hpp"
#include "application/cliparser.hpp"
#include "adapters/namedpipe/namedpipecommandinterface.hpp"
#include "config.hpp"
#include "domain/ports/kakounesession.hpp"
#include "kakodemon_integration_kakscript.hpp"
#include <iostream>
#include <memory>

void printHelp(const char* program_name)
{
    std::cout << "Usage: " << program_name << " [OPTIONS] [file]... [+<line>[:<col>]|+:]\n\n"
              << "Options:\n"
              << "  -c <session>      Connect to remote Kakoune session\n"
              << "  -C <session>      Connect to remote Kakoune session, create local session if it does not exist\n"
              << "  -s <session>      Set session name\n"
              << "  -e <command>      Execute command on client initialisation\n"
              << "  -p <command>      Send command to a running instance (uses KAKOD_ID env)\n"
              << "  -n                Do not source kakrc files on startup\n"
              << "  --version         Print version and exit\n"
              << "  --init            Print the Kakodemon integration script and exit\n"
              << "  --help            Print this help message and exit\n\n"
              << "File arguments:\n"
              << "  [file]...         Files to edit\n"
              << "  [+<line>[:<col>]] Edit file at specific line (and column)\n"
              << "  [+:]              Edit file at last position\n";
}

void printVersion()
{
    std::cout << "kakodemon " << KAKODEMON_VERSION << "\n";
}

void printKakodemonPluginScript()
{
    std::cout << kakodemon_integration_kakscript;
}

int main(int argc, char* argv[])
{
    CliParser parser(ValidatorDependencies{ .kakouneSessionExists = domain::kakouneSessionExists });
    ParsedCliArgs parsed_args = parser.parseAndValidate(argc, argv);

    switch (parsed_args.result)
    {
        case ParseResult::ShowVersion:
            printVersion();
            return 0;
        case ParseResult::ShowHelp:
            printHelp(argv[0]);
            return 0;
        case ParseResult::ShowKakodemonIntegrationScript:
            printKakodemonPluginScript();
            return 0;
        case ParseResult::SendCommand:
        {
            const char* pipe_id = std::getenv("KAKOD_ID");
            if (!pipe_id)
            {
                std::cerr << "Error: KAKOD_ID environment variable not set\n";
                return 1;
            }

            NamedPipeCommandInterface interface(std::string(pipe_id), PipeMode::Send);
            domain::Command cmd;
            cmd.name = parsed_args.command_request.command;
            cmd.args = parsed_args.command_request.args;
            if (!interface.sendCommand(cmd)) {
                std::cerr << "Error: Failed to send command\n";
                return 1;
            }
            return 0;
        }
        case ParseResult::Error:
            std::cerr << "Error: " << parsed_args.error_message << "\n";
            return 1;
        case ParseResult::Success:
            break;
    }

    ApplicationConfig app_config = loadApplicationConfig();

    ApplicationBuilder builder;
    auto app = builder
        .withPlatform(PlatformType::OPENGL_GLFW)
        .withFontResolver(FontResolverType::FONTCONFIG)
        .withFontEngine(FontEngineType::FREETYPE)
        .build();

    app->init(parsed_args.config, app_config);
    app->run();
    return 0;
}
