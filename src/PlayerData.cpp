#include <cstddef> // size_t definition
#include "PlayerData.h"

namespace Citadel
{
#pragma region PlayerNumber
    const size_t GetMinimumPlayers()
    {
        return 2;
    }
    const size_t GetMaximumPlayers()
    {
        return 6;
    }
    const bool ValidateNumberOfPlayers(const size_t humanPlayers, const size_t robotPlayers)
    {
        if (humanPlayers > GetMaximumPlayers() || robotPlayers > GetMaximumPlayers())
        {
            return false;
        }

        const size_t totalPlayers = humanPlayers + robotPlayers;
        if (totalPlayers >= GetMinimumPlayers() && totalPlayers <= GetMaximumPlayers())
        {
            return true;
        }
        return false;
    }
#pragma endregion

#pragma region PlayerTurnStep
    namespace
    {
        const char* const playerTurnStepNames[] =
        {
            "UNINITIALIZED",
            "ACTION_STEP",
            "BUILD_STEP",
            "MAGIC_POWER_STEP",
            "DISTRICT_INCOME_STEP",
        };
    }

    const char* const GetPlayerTurnStepName(const PlayerTurnStep step)
    {
        static_assert(static_cast<size_t>(PlayerTurnStep::MAX) == sizeof(playerTurnStepNames) / sizeof(playerTurnStepNames[0]),
            "PlayerTurnStep::MAX must match playerTurnStepNames number of items");
        return playerTurnStepNames[static_cast<size_t>(step)];
    }
#pragma endregion

#pragma region PlayerAction
    namespace
    {
        const char* const playerActionNames[] =
        {
            "UNINITIALIZED",
            "TAKE GOLD COINS",
            "WATCH DISTRICT CARDS",
            "BUILD DISTRICT CARDS",
            "USE MAGIC POWER",
            "EARN DISTRICT INCOME",
        };
    }

    const char* const GetPlayerActionName(const PlayerAction action)
    {
        static_assert(static_cast<size_t>(PlayerAction::UNINITIALIZED) == 0, "PlayerAction::UNINITIALIZED must be first element and set to 0");
        static_assert(static_cast<size_t>(PlayerAction::MAX) == sizeof(playerActionNames) / sizeof(playerActionNames[0]),
            "PlayerAction::MAX must match playerActionNames number of items");
        return playerActionNames[static_cast<size_t>(action)];
    }
#pragma endregion
}