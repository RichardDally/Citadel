#include <cassert>
#include <iostream>
#include "CharacterDeck.h"

namespace Citadel
{
    void CharacterDeck::Setup(std::initializer_list<Character>&& availableCharacters, const size_t numberOfPlayers)
    {
        availableCharacters_ = availableCharacters;
        numberOfPlayers_ = numberOfPlayers;
        faceupCards_.clear();
        remainingCards_.clear();
    }

    void CharacterDeck::RemoveCharactersStep()
    {
        // Cleanup
        faceupCards_.clear();
        remainingCards_.clear();

        // Copy every available cards to remaining ones
        remainingCards_.insert(std::begin(availableCharacters_), std::end(availableCharacters_));

        // Define some rules as lambda functions
        auto faceupSpecialRule = [] (const Character c)
        {
            return c == Character::KING ? false : true;
        };
        auto faceoffRule = [](const Character)
        {
            return true;
        };

        assert(numberOfPlayers_ >= 2 && numberOfPlayers_ <= 7);
        switch (numberOfPlayers_)
        {
            case 4:
            {
                assert(remainingCards_.size() == 8);
                // Withdraw 1 character card to faceoff heap
                WithdrawCards(1, faceoffCards_, faceoffRule);
                // Withdraw 2 character cards to faceup heap
                WithdrawCards(2, faceupCards_, faceupSpecialRule);
                break;
            }
            case 5:
            {
                assert(remainingCards_.size() == 8);
                // Withdraw 1 character card to faceoff heap
                WithdrawCards(1, faceoffCards_, faceoffRule);
                // Withdraw 1 character card to faceup heap
                WithdrawCards(1, faceupCards_, faceupSpecialRule);
                break;
            }
            case 6:
            case 7:
            {
                assert(remainingCards_.size() == 8);
                // No character faceup cards
                break;
            }
            default:
            {
            }
        }

        // There must be remaining cards to pick by players
        assert(remainingCards_.size() > numberOfPlayers_);
    }
}