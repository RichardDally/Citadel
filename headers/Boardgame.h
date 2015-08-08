#pragma once

#include <vector>
#include <memory>

#include "Player.h"
#include "CharacterDeck.h"

namespace Citadel
{
    class Boardgame
    {
    public:
        Boardgame();
        // TODO: Add ctor to customize game
        // TODO: Add reset function

        void StartBasicGame();

    private:
        void StartRound();

        // Round Step 1
        void RemoveCharactersStep();

        // Round Step 2
        void ChooseCharactersStep();

        // Round Step 3
        void PlayerTurnsStep();

        // Round Step 4
        void EndOfTurnStep();

        bool IsGameEnded() const;

        template <typename PlayerType>
        void AddPlayer(const size_t n)
        {
            static_assert(std::is_same<PlayerType, HumanPlayer>::value || std::is_same<PlayerType, RobotPlayer>::value,
                "PlayerType must be HumanPlayer or RobotPlayer");
            for (size_t i = 0; i < n; ++i)
            {
                players_.emplace_back(new PlayerType);
            }
        }

        size_t currentRound_ = 0;
        size_t startingPlayer_ = 0;
        size_t currentPlayer_ = 0;
        size_t nextStartingPlayer_ = 0;

        CharacterDeck characterDeck_;
        std::vector<std::unique_ptr<Player>> players_;
    };
}