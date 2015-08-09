#pragma once

#include <vector>
#include <iostream>
#include <functional>
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
        CharacterDeck(const CharacterDeck&) = default;
        CharacterDeck& operator=(const CharacterDeck&) = default;

        void Setup(std::initializer_list<Character>&& availableCharacters, const size_t numberOfPlayers);
        void RemoveCharactersStep();
        void ChooseCharactersStep();

        const std::unordered_set<Character>& GetFaceupCards() const
        {
            return faceupCards_;
        }

        const std::unordered_set<Character>& GetRemainingCards() const
        {
            return remainingCards_;
        }

        void RemoveCard(const Character character)
        {
            std::cout << "Debug: remove [" << GetCharacterName(character) << "] from remaining cards." << std::endl;
            remainingCards_.erase(character);
        }

    private:
        // Move character cards from remaining heap to faceup or faceoff heaps.
        template <typename DestContainer>
        void WithdrawCards(const size_t n, DestContainer& container, std::function<bool(const Character)> characterFilter)
        {
            const auto initialSize = container.size();
            while (remainingCards_.size() > 0 && container.size() - initialSize != n)
            {
                // Pick random iterator within unordered_set
                auto it = std::next(std::begin(remainingCards_), Dice::GetRandomNumber(0, remainingCards_.size() - 1));
                const auto character = *it;

                if (characterFilter(character))
                {
                    std::cout << "Debug: " << (&container == &faceupCards_ ? "faceup" : "faceoff") << " character: " << GetCharacterName(character) << std::endl;
                    container.insert(character);
                    remainingCards_.erase(it);
                }
            }
        }

        size_t numberOfPlayers_ = 0;
        std::vector<Character> availableCharacters_; // referential, readonly

        std::unordered_set<Character> faceupCards_;
        std::unordered_set<Character> faceoffCards_;
        std::unordered_set<Character> remainingCards_;
    };
}
