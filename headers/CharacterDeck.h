#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <set>

#include "Logger.h"
#include "CharacterData.h"
#include "Randomness.h"

namespace Citadel
{
    class CharacterDeck
    {
    public:
        CharacterDeck() = default;
        ~CharacterDeck() = default;
        CharacterDeck(const CharacterDeck&) = default;
        CharacterDeck& operator=(const CharacterDeck&) = default;

        void Setup(const std::vector<Character>& availableCharacters, const size_t numberOfPlayers);
        void RemoveCharactersStep(const Edition edition);
        void ChooseCharactersStep();

        // Compute possible opponents of a specific character (useful for Assassin or Thief)
        std::set<Character> PossibleOpponentsCharacters(const Character playerCharacter);

        const std::set<Character>& GetFaceupCards() const
        {
            return faceupCards_;
        }

        const std::set<Character>& GetRemainingCards() const
        {
            return remainingCards_;
        }

        void RemoveCard(const Character character)
        {
            Logger::GetInstance() << Verbosity::DEBUG << "remove [" << GetCharacterName(character) << "] from remaining cards." << std::endl;
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
                    Logger::GetInstance() << Verbosity::DEBUG << (&container == &faceupCards_ ? "faceup" : "faceoff") << " character: " << GetCharacterName(character) << std::endl;
                    container.insert(character);
                    remainingCards_.erase(it);
                }
            }
        }

        size_t numberOfPlayers_ = 0;
        std::vector<Character> availableCharacters_; // referential, readonly

        // C++11 note: std::unordered_set does not work with enum class as key.
        // => This is fixed in C++14.
        // TODO C++14: change to std::unordered_set
        std::set<Character> faceupCards_;
        std::set<Character> faceoffCards_;
        std::set<Character> remainingCards_;
    };
}
