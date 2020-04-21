#include <iostream>
#include "ConsoleMenu.h"

int main()
{
    // TODO: parse args
    try
    {
        Citadel::ConsoleMenu::Start();
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << "\n";
    }
    
    return 0;
}