#include <cassert>
#include <iostream>
#include "CharacterDeck.h"

namespace Citadel
{
    namespace
    {
        // Define some rules as lambda functions
        auto faceupSpecialRule = [](const Character c)
        {
            return c == Character::KING ? false : true;
        };
        auto faceoffRule = [](const Character)
        {
            return true;
        };
    }

    void CharacterDeck::Setup(const std::vector<Character>& availableCharacters, const size_t numberOfPlayers)
    {
        availableCharacters_ = availableCharacters;
        numberOfPlayers_ = numberOfPlayers;
        faceupCards_.clear();
        faceoffCards_.clear();
        remainingCards_.clear();
    }

    void CharacterDeck::RemoveCharactersStep(const Edition edition)
    {
        // Cleanup
        faceupCards_.clear();
        faceoffCards_.clear();
        remainingCards_.clear();

        // Copy every available cards to remaining ones
        remainingCards_.insert(std::begin(availableCharacters_), std::end(availableCharacters_));

        assert(numberOfPlayers_ >= 2 && numberOfPlayers_ <= 7);
        switch (numberOfPlayers_)
        {
            case 4:
            {
                assert(remainingCards_.size() == 8);
                WithdrawCards(1, faceoffCards_, faceoffRule);
                WithdrawCards(2, faceupCards_, faceupSpecialRule);
                break;
            }
            case 5:
            {
                assert(remainingCards_.size() == 8);
                WithdrawCards(1, faceoffCards_, faceoffRule);
                WithdrawCards(1, faceupCards_, faceupSpecialRule);
                break;
            }
            case 6:
            {
                assert(remainingCards_.size() == 8);
                WithdrawCards(1, faceoffCards_, faceoffRule);
                break;
            }
            default:
            {
                assert(!"Not implemented");
            }
        }

        // There must be remaining cards to pick for players
        assert(remainingCards_.size() > numberOfPlayers_);
    }

    // Each player picked a role, remaining cards are going faceoff
    void CharacterDeck::ChooseCharactersStep()
    {
        WithdrawCards(remainingCards_.size(), faceoffCards_, faceoffRule);
        assert(remainingCards_.empty());
    }

    // Compute possible opponents of a specific character (useful for Assassin or Thief)
    std::set<Character> CharacterDeck::PossibleOpponentsCharacters(const Character playerCharacter)
    {
        assert(playerCharacter != Character::UNINITIALIZED);
        std::set<Character> result;

        for (const auto character : availableCharacters_)
        {
            // Cannot self target
            if (character != playerCharacter)
            {
                // Cannot target faced up characters
                if (faceupCards_.find(character) == std::end(faceupCards_))
                {
                    // Cannot steal Assassin
                    if (playerCharacter == Character::THIEF && character == Character::ASSASSIN)
                    {
                    }
                    else
                    {
                        result.insert(character);
                    }
                }
            }
        }

        return result;
    }
}