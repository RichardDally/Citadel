#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "HumanPlayer.h" // for static_assert
#include "RobotPlayer.h" // for static_assert
#include "CharacterDeck.h"
#include "DistrictDeck.h"

namespace Citadel
{
    class Boardgame
    {
    public:
        Boardgame();
        // TODO: Add ctor to customize game

        void StartBasicGame();

    private:
        // Transfer district cards to specific player
        void TransferDistrictCards(const size_t numberOfCards, Player* player);

        // Returns player id
        int DecideWhoStarts();

        void StartRound();

        // Round Step 1
        void RemoveCharactersStep();

        // Round Step 2
        void ChooseCharactersStep();

        // Round Step 3
        void PlayerTurnsStep();
        // Sub functions
        bool CanUseMagicPower(const Character character) const;
        bool AskCharacterTarget(Player* player, Character& victim);
        bool AssassinMagicPower(Player* player, Character& murderedCharacter);
        bool ThiefMagicPower(Player* player, Character& stolenCharacter);
        bool MagicianMagicPower(Player* player);
        bool MagicianExchangeFromPlayer(Player* player);
        bool MagicianExchangeFromDistrictDeck(Player* player);
        bool WarlordMagicPower(Player* player);
        void EarnGoldFromDistricts(const Character character, Player* player);
        void PlayerTurn(Player* player, Character& murderedCharacter, Character& stolenCharacter);
        bool UseMagicPower(Player* player, Character& murderedCharacter, Character& stolenCharacter);

        // Round Step 4
        void EndOfTurnStep();

        bool IsGameEnded() const;

        template <typename PlayerType>
        void AddPlayer(const size_t n)
        {
            assert(n > 0);
            static_assert(std::is_same<PlayerType, HumanPlayer>::value || std::is_same<PlayerType, RobotPlayer>::value,
                "PlayerType must be HumanPlayer or RobotPlayer");
            for (size_t i = 0; i < n; ++i)
            {
                // TODO: replace by std::make_unique
                auto newPlayer = std::unique_ptr<PlayerType>(new PlayerType);
                playerById_.insert(std::make_pair(newPlayer->GetID(), std::move(newPlayer)));
            }
        }

        size_t currentRound_ = 0;
        int startingPlayer_ = 0;
        int currentPlayer_ = 0;
        int nextStartingPlayer_ = 0;

        CharacterDeck characterDeck_;
        DistrictDeck districtDeck_;
        std::unordered_map<int, std::unique_ptr<Player>> playerById_;
        std::map<Character, Player*> playerByCharacter_;
    };
}