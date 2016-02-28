#include <set>
#include <iostream>
#include "Boardgame.h"
#include "PlayerData.h"
#include "ConsoleMenu.h"

namespace Citadel
{
    void ConsoleMenu::Start()
    {
        Edition edition = Edition::UNINITIALIZED;
        do
        {
            edition = ConsoleMenu::ChooseEdition();
        }
        while (edition == Edition::UNINITIALIZED);

        Boardgame boardgame;

        size_t humanPlayers, robotPlayers;
        ChoosePlayerNumbers(humanPlayers, robotPlayers);

        for (size_t i = 0; i < humanPlayers; ++i)
        {
            std::string name;
            do
            {
                std::cout << "Type human player name (" << i << ")" << std::endl;
                std::cin >> name;
            }
            while (name.empty());
            boardgame.AddPlayer<HumanPlayer>(name);
        }
        for (size_t i = 0; i < robotPlayers; ++i)
        {
            std::string name;
            do
            {
                std::cout << "Type robot player name (" << i << ")" << std::endl;
                std::cin >> name;
            }
            while (name.empty());
            boardgame.AddPlayer<RobotPlayer>(name);
        }

        boardgame.StartGame(edition);
    }

    const Edition ConsoleMenu::ChooseEdition()
    {
        // TODO: remove this container when these editions are implemented
        std::set<Edition> notImplementedEditions { Edition::REGULAR, Edition::DARK_CITY };

        std::cout << "Choose edition among:" << std::endl;
        for (size_t i = 1; i < static_cast<size_t>(Edition::MAX); ++i)
        {
            if (notImplementedEditions.find(static_cast<Edition>(i)) == std::end(notImplementedEditions))
            {
                std::cout << "- " << GetEditionName(static_cast<Edition>(i)) << " (" << i << ")" << std::endl;
            }
        }

        size_t editionIndex = 0;
        std::cin >> editionIndex;

        if (editionIndex >= 1 && editionIndex < static_cast<size_t>(Edition::MAX))
        {
            return static_cast<Edition>(editionIndex);
        }

        return Edition::UNINITIALIZED;
    }

    void ConsoleMenu::ChoosePlayerNumbers(size_t& humanPlayers, size_t& robotPlayers)
    {
        do
        {
            std::cout << "Minimum players: " << GetMinimumPlayers() << std::endl;
            std::cout << "Maximum players: " << GetMaximumPlayers() << std::endl;

            std::cout << "Choose number of human players: ";
            std::cin >> humanPlayers;

            std::cout << "Choose number of robot players: ";
            std::cin >> robotPlayers;
        }
        while (ValidateNumberOfPlayers(humanPlayers, robotPlayers) == false);
    }
}
