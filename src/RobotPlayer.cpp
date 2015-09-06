#include "Logger.h"
#include "Randomness.h"
#include "RobotPlayer.h"

namespace Citadel
{
    namespace
    {
        const char* const names[] =
        {
            "Hal",
            "Synapse",
            "DevNull",
            "Alpha",
            "TheOracle",
            "Alfred",
            "Corona",
        };
        const size_t namesSize = sizeof(names) / sizeof(names[0]);
    }

    RobotPlayer::RobotPlayer()
        : Player(names[Dice::GetRandomNumber(0, namesSize - 1)])
    {
    }

    RobotPlayer::RobotPlayer(const std::string& name)
        : Player(name)
    {
    }

#pragma region PURE VIRTUAL METHODS
    // Returns character picked to play
    Character RobotPlayer::PickCharacter(const std::set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    // Returns action to be taken
    PlayerAction RobotPlayer::ChooseAction(const std::vector<PlayerAction>& availableActions)
    {
        assert(!"Boom");
        return PlayerAction::UNITIALIZED;
    }

    District RobotPlayer::WatchAndChooseDistrictCard(const std::vector<District>& districts)
    {
        assert(!"Boom");
        return District::UNINITIALIZED;
    }

    // Returns districts player wants to build
    std::vector<District> RobotPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        assert(!"Boom");
        return std::vector<District>();
    }

    // Returns character targeted by assassination or theft
    Character RobotPlayer::ChooseCharacterTarget(const std::set<Character>& opponents)
    {
        assert(!"Boom");
        return Character::UNINITIALIZED;
    }

    // Returns opponent player id, current player wants to target
    int RobotPlayer::ChoosePlayerTarget(std::vector<const Player*> opponents)
    {
        assert(!"Boom");
        return -1;
    }

    // Returns a pair containing player id (self district destroy is tolerated) as key and destroyed district as value
    std::pair<int, District> RobotPlayer::ChoosePlayerDistrictTarget(std::vector<const Player*> players)
    {
        assert(!"Boom");
        return std::pair<int, District>(-1, District::UNINITIALIZED);
    }

    // Returns a choice specific to Magician character
    MagicianChoice RobotPlayer::MagicianDecision()
    {
        assert(!"Boom");
        return MagicianChoice::DO_NOTHING;
    }

    // Returns discarded districts to be replaced by equivalent number of cards from district deck
    std::vector<District> RobotPlayer::ChooseDistrictsCardsToSwap()
    {
        assert(!"Boom");
        return std::vector<District>();
    }
#pragma endregion
}