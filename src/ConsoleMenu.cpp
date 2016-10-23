#include <set>
#include <iostream>
#include "Logger.h"
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
        static_assert(static_cast<size_t>(Edition::UNINITIALIZED) == 0, "Edition::UNINITIALIZED must be first and equal to zero");
        static const size_t firstAvailableEdition = static_cast<size_t>(Edition::UNINITIALIZED) + 1;

        // TODO: remove this container when these editions are implemented
        static const std::set<Edition> notImplementedEditions { Edition::REGULAR, Edition::DARK_CITY };

        size_t pickedEdition = static_cast<size_t>(Edition::UNINITIALIZED);
        size_t displayedEditions = 0;
        std::cout << "Choose edition among:\n";
        size_t editionIndex = static_cast<size_t>(Edition::UNINITIALIZED) + 1;
        for (; editionIndex < static_cast<size_t>(Edition::MAX); ++editionIndex)
        {
            if (notImplementedEditions.find(static_cast<Edition>(editionIndex)) == std::end(notImplementedEditions))
            {
                std::cout << "- " << GetEditionName(static_cast<Edition>(editionIndex)) << " (" << editionIndex << ")\n";
                pickedEdition = editionIndex;
                ++displayedEditions;
            }
        }

        if (displayedEditions > 1)
        {
            Logger::GetInstance() << Verbosity::DEBUG << displayedEditions << " editions are available" << std::endl;
            std::cin >> pickedEdition;
        }
        else
        {
            Logger::GetInstance() << Verbosity::DEBUG << "Skipping choice, only " << GetEditionName(static_cast<Edition>(pickedEdition)) << " is available" << std::endl;
        }

        if (pickedEdition >= firstAvailableEdition && pickedEdition < static_cast<size_t>(Edition::MAX))
        {
            return static_cast<Edition>(pickedEdition);
        }

        return Edition::UNINITIALIZED;
    }

    void ConsoleMenu::ChoosePlayerNumbers(size_t& humanPlayers, size_t& robotPlayers)
    {
        do
        {
            std::cout << "Minimum total players: " << GetMinimumPlayers() << "\n";
            std::cout << "Maximum total players: " << GetMaximumPlayers() << "\n";

            std::cout << "Choose number of human players: ";
            std::cin >> humanPlayers;

            std::cout << "Choose number of robot players: ";
            std::cin >> robotPlayers;
        }
        while (ValidateNumberOfPlayers(humanPlayers, robotPlayers) == false);
    }
}
