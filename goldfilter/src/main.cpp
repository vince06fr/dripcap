#include <sstream>
#include <string>
#include <spdlog/spdlog.h>

#include "executable.hpp"
#include "server.hpp"

int main(int argc, char *argv[])
{
    const char *loglevel = std::getenv("GOLDFILTER_LOG");
    if (loglevel && strcmp(loglevel, "debug") == 0)
        spdlog::set_level(spdlog::level::debug);
    else
        spdlog::set_level(spdlog::level::err);

    auto spd = spdlog::stdout_logger_mt("console", true);

    std::string command;
    if (argc == 2)
        command.assign(argv[1]);

    spd->debug("command: {}", command);

    if (command.empty()) {
        return EXIT_FAILURE;
    } else if (command == "--test-perm") {
        bool result = Executable().testPermission();
        spd->info("--test-perm: {}", result);
        return result ? EXIT_SUCCESS : EXIT_FAILURE;
    } else if (command == "--set-perm") {
        bool result = Executable().grantPermission();
        spd->info("--set-perm: {}", result);
        return result ? EXIT_SUCCESS : EXIT_FAILURE;
    } else if (command.find("-") == 0) {
        spd->error("unrecognized option: {}", command);
        return EXIT_FAILURE;
    }

    Server server(command);
    return server.start() ? EXIT_SUCCESS : EXIT_FAILURE;
}
