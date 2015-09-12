#pragma once

#include <utility>
#include "GameData.h"

namespace Citadel
{
    class ConsoleMenu
    {
    public:
        static void Start();

    private:
        static const Edition ChooseEdition();
        static size_t ChoosePlayerNumbers(size_t& humanPlayers, size_t& robotPlayers);
    };
}