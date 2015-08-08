#pragma once

#include <vector>
#include <unordered_set>
#include "Randomness.h"
#include "CharacterCard.h"

namespace Citadel
{
    class CharacterDeck
    {
    public:
        CharacterDeck() = default;
        ~CharacterDeck() = default;

        void Setup(std::initializer_list<Character>&& availableCharacters, const size_t numberOfPlayers);
        void RemoveCharactersStep();
        void FaceupCharacterCards(const size_t n);

        const std::unordered_set<Character>& GetRemainingCards() const
        {
            return remainingCards_;
        }

        void RemoveCard(const Character character)
        {
            std::cout << "[Debug] Remove [" << GetCharacterName(character) << "] from remaining cards." << std::endl;
            remainingCards_.erase(character);
        }

    private:
        size_t numberOfPlayers_ = 0;
        std::vector<Character> availableCharacters_;

        std::unordered_set<Character> faceupCards_;
        std::unordered_set<Character> hiddenCards_;
        std::unordered_set<Character> remainingCards_;
    };
}
