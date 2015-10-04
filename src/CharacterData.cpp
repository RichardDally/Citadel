#include <cassert>
#include <iostream>
#include "Logger.h"
#include "CharacterData.h"

namespace Citadel
{
#pragma region Character
    namespace
    {
        const char* const characterNames[] =
        {
            "UNINITIALIZED",
            "ASSASSIN",
            "THIEF",
            "MAGICIAN",
            "KING",
            "BISHOP",
            "MERCHANT",
            "ARCHITECT",
            "WARLORD"
        };

        const char* const characterDescriptions[] =
        {
            "No description",
            "Tremendously handy for shanking those greedy Merchants who try to get rich quick",
            "Why work hard to amass gold when you could just steal it ?",
            "Like a certain Spanish Inquisition, no one ever expects the Magician.",
            "Looking down on you with snooty horror, the King wonders why you've dared invade his chambers with your filthy person, one hand poised to make a dismissive \"flicking\" motion in your general direction.",
            "Practically a license to print money for the enterprising builder with lots of religious districts in his city.",
            "Merchant exists to fill the coffers of the enterprising player, and he does it very well.",
            "There's no better way to build up a monster hand of cards than with the Architect.",
            "Smug and confident in his ability to kill you at pretty much any moment, the Warlord sits back and relaxes as he ponders how best to destroy you."
        };
    }

    const char* const GetCharacterName(const Character character)
    {
        static_assert(static_cast<size_t>(Character::MAX) == sizeof(characterNames) / sizeof(characterNames[0]),
            "Character::MAX must match characterNames number of items");
        return characterNames[static_cast<size_t>(character)];
    }

    const char* const GetCharacterDescription(const Character character)
    {
        static_assert(static_cast<size_t>(Character::MAX) == sizeof(characterDescriptions) / sizeof(characterDescriptions[0]),
            "Character::MAX must match characterDescriptions number of items");
        return characterDescriptions[static_cast<size_t>(character)];
    }

    const Color GetCharacterColor(const Character character)
    {
        Color result = Color::UNINITIALIZED;

        switch (character)
        {
            case Character::ASSASSIN: break;
            case Character::THIEF: break;
            case Character::MAGICIAN: break;
            case Character::ARCHITECT: break;
            case Character::KING:
            {
                result = Color::YELLOW;
                break;
            }
            case Character::BISHOP:
            {
                result = Color::BLUE;
                break;
            }
            case Character::MERCHANT:
            {
                result = Color::GREEN;
                break;
            }
            case Character::WARLORD:
            {
                result = Color::RED;
                break;
            }
            default:
            {
                Logger::GetInstance() << Verbosity::ERROR << "Character [" << static_cast<int>(character) << "] is not handled." << std::endl;
                assert(!"This character is not handled to get it's color");
            }
        }

        return result;
    }

    const std::vector<Character>& GetCharacterCallingOrder(const Edition edition)
    {
        static const std::vector<Character> dummy;

        switch (edition)
        {
            case Edition::REGULAR_WITHOUT_PURPLE_DISTRICTS:
            {
                static const std::vector<Character> callingOrder
                {
                    Character::ASSASSIN,
                    Character::THIEF,
                    Character::MAGICIAN,
                    Character::KING,
                    Character::BISHOP,
                    Character::MERCHANT,
                    Character::ARCHITECT,
                    Character::WARLORD,
                };
                return callingOrder;
            }
            default:
            {
                assert(!"Not implemented");
            }
        }

        return dummy;
    }

    
#pragma endregion

#pragma region MagicianChoice
    namespace
    {
        const char* const magicianChoiceNames[] =
        {
            "UNINITIALIZED",
            "EXCHANGE_FROM_PLAYER",
            "EXCHANGE_FROM_DISTRICT_DECK",
            "DO_NOTHING",
        };
    }

    const char* const GetMagicianChoiceName(const MagicianChoice choice)
    {
        static_assert(static_cast<size_t>(MagicianChoice::MAX) == sizeof(magicianChoiceNames) / sizeof(magicianChoiceNames[0]),
            "MagicianChoice::MAX must match magicianChoiceNames number of items");
        return magicianChoiceNames[static_cast<size_t>(choice)];
    }
}
#pragma endregion