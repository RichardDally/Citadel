#pragma once

namespace Citadel
{
#pragma region PlayerNumber
    const size_t GetMinimumPlayers();
    const size_t GetMaximumPlayers();
    const bool ValidateNumberOfPlayers(const size_t humanPlayers, const size_t robotPlayers);
#pragma endregion

#pragma region PlayerTurnStep
    enum class PlayerTurnStep
    {
        UNINITIALIZED,
        ACTION_STEP,
        BUILD_STEP,
        MAGIC_POWER_STEP,
        DISTRICT_INCOME_STEP,
        MAX
    };
    const char* const GetPlayerTurnStepName(const PlayerTurnStep step);
#pragma endregion

#pragma region PlayerAction
    enum class PlayerAction
    {
        UNINITIALIZED,
        TAKE_GOLD_COINS,
        WATCH_DISTRICT_CARDS,
        BUILD_DISTRICT_CARDS,
        USE_MAGIC_POWER,
        EARN_DISTRICT_INCOME,
        MAX
    };
    const char* const GetPlayerActionName(const PlayerAction action);
#pragma endregion
}