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

        assert(numberOfPlayers_ >= 2 && numberOfPlayers_ <= 7);
        switch (numberOfPlayers_)
        {
            case 4:
            {
                assert(availableCharacters_.size() == 8);
                // 2 character cards faceup
                FaceupCharacterCards(2);
                break;
            }
            case 5:
            {
                assert(availableCharacters_.size() == 8);
                // 1 character card faceup
                FaceupCharacterCards(1);
                break;
            }
            case 6:
            case 7:
            {
                assert(availableCharacters_.size() == 8);
                // No character faceup cards
                break;
            }
            default:
            {
            }
        }

        // Fill remainingCards_ accordingly with faceup cards
        for (const auto character : availableCharacters_)
        {
            if (faceupCards_.find(character) == faceupCards_.end())
            {
                remainingCards_.insert(character);
            }
        }

        assert(remainingCards_.size() > 0);
    }

    void CharacterDeck::FaceupCharacterCards(const size_t n)
    {
        assert(faceupCards_.empty());
        while (faceupCards_.size() != n)
        {
            const auto index = Dice::GetRandomNumber(0, availableCharacters_.size());
            const auto character = availableCharacters_[index];
            // Special rule: king cannot be faceup
            if (character != Character::KING)
            {
                std::cout << "[Debug] Faceup character: " << GetCharacterName(character) << std::endl;
                faceupCards_.insert(character);
            }
        }
    }
}