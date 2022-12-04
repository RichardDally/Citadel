#include <set>
#include <iostream>
#include <unordered_set>
#include "Boardgame.h"
#include "PlayerData.h"
#include "ConsoleMenu.h"
#include "Randomness.h"


namespace Citadel
{
    void ConsoleMenu::Start()
    {
        Edition edition = Edition::REGULAR_WITHOUT_PURPLE_DISTRICTS;
        /*
        do
        {
            edition = ConsoleMenu::ChooseEdition();
        }
        while (edition == Edition::UNINITIALIZED);
        */

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

        std::vector<std::string> robotNames =
        {
            "Minotaur",
            "Todd",
            "HAL",
            "Frost",
            "Karl V",
            "42",
            "Golem 9000"
        };

        // TODO: turn this check into constexpr/static_assert
        if (GetMaximumPlayers() > robotNames.size())
        {
            throw std::logic_error("Not enough robot names");
        }

        for (size_t i = 0; i < robotPlayers; ++i)
        {
            const int robotNameIndex = Dice::GetRandomNumber(0, robotNames.size() - 1);
            const auto& robotName = robotNames.at(robotNameIndex);
            boardgame.AddPlayer<RobotPlayer>(robotName);
            robotNames.erase(robotNames.begin() + robotNameIndex);
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
            spdlog::debug("[{}] editions are available", displayedEditions);
            std::cin >> pickedEdition;
        }
        else
        {
            spdlog::debug("Skipping choice, only [{}] is available", GetEditionName(static_cast<Edition>(pickedEdition)));
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
