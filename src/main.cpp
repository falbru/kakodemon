#include "application/application.hpp"
#include "application/cliparser.hpp"
#include "adapters/opengl/application.hpp"
#include "config.hpp"
#include <memory>
#include <iostream>

void print_help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n"
              << "Options:\n"
              << "  -c <session>      Connect to remote Kakoune session\n"
              << "  -s <session>      Set session name\n"
              << "  -e <command>      Execute command on client initialisation\n"
              << "  --version         Print version and exit\n"
              << "  --help            Print this help message and exit\n";
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
        case ParseResult::Error:
            std::cerr << parsed_args.error_message << "\n";
            return 1;
        case ParseResult::Success:
            break;
    }

    std::unique_ptr<Application> app = std::make_unique<opengl::GLFWApplication>();
    app->init(parsed_args.config);
    app->run();
    return 0;
}
