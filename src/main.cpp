#include <iostream>
#include <spdlog/spdlog.h>
#include "ConsoleMenu.h"


int main()
{
    spdlog::set_level(spdlog::level::debug);
    // TODO: parse args
    try
    {
        Citadel::ConsoleMenu::Start();
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << "\n";
    }
}
