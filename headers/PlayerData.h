#pragma once

namespace Citadel
{
#pragma region PlayerNumber
    const size_t GetMinimumPlayers();
    const size_t GetMaximumPlayers();
#pragma endregion

#pragma region PlayerTurnStep
    enum class PlayerTurnStep
    {
        UNINITIALIZED,
        ACTION_STEP,
        BUILD_STEP,
        MAGIC_POWER_STEP,
        ENDING_STEP,
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
        MAX
    };
    const char* const GetPlayerActionName(const PlayerAction action);
#pragma endregion
}