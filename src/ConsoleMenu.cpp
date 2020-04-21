#include <set>
#include <iostream>
#include "Boardgame.h"
#include "PlayerData.h"
#include "ConsoleMenu.h"

namespace Citadel
{
    void ConsoleMenu::Start()
    {
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

        static const char* const names[] =
        {
            "Hal",
            "Synapse",
            "DevNull",
            "Alpha",
            "TheOracle",
            "Alfred",
            "Corona",
        };

        for (size_t i = 0; i < robotPlayers; ++i)
        {
            boardgame.AddPlayer<RobotPlayer>(names[i]);
        }

        // TODO: call ChooseEdition() when multiple editions are available (currently one)
        const Edition edition = Edition::REGULAR_WITHOUT_PURPLE_DISTRICTS;

        boardgame.StartGame(edition);
    }

    const Edition ConsoleMenu::ChooseEdition()
    {
        Edition edition = Edition::UNINITIALIZED;
        do
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
                edition = static_cast<Edition>(editionIndex);
            }
        }
        while (edition == Edition::UNINITIALIZED);

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
