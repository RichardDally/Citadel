#include <iostream>
#include <spdlog/spdlog.h>
#include <cxxopts/cxxopts.hpp>
#include "ConsoleMenu.h"


int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);

    cxxopts::Options options(argv[0]);
    options.positional_help("[optional args]").show_positional_help();

    options
    .allow_unrecognised_options()
    .add_options()
    ("i,interactive", "Interactive game", cxxopts::value<bool>()->default_value("false"))
    ("a,automated_game", "Set AI players count (min 2, max 6)", cxxopts::value<std::uint16_t>()->default_value("6"))
    ("h,help", "Print help")
    ;

    const auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help({ "", "Group" }) << std::endl;
        exit(0);
    }

    try
    {
        if (result["interactive"].as<bool>())
        {
            Citadel::ConsoleMenu::StartInteractiveGame();
        }
        else
        {
            Citadel::ConsoleMenu::StartAIGame(result["automated_game"].as<std::uint16_t>());
        }
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << "\n";
    }
}
