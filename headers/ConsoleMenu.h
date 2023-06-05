#pragma once

#include <cstddef> // size_t definition
#include <utility>
#include "GameData.h"

namespace Citadel
{
    class ConsoleMenu
    {
    public:
        static void StartAIGame(const std::uint16_t robotPlayers);
        static void StartInteractiveGame();

    private:
        static const Edition ChooseEdition();
        static void ChoosePlayerNumbers(size_t& humanPlayers, size_t& robotPlayers);
    };
}
