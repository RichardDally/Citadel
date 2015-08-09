#include "Randomness.h"
#include "RobotPlayer.h"

namespace
{
    const char* const names[] =
    {
        "Hal",
        "Synapse",
        "DevNull",
        "Alpha",
        "TheOracle"
    };
    const size_t namesSize = sizeof(names) / sizeof(names[0]);
}

namespace Citadel
{
    RobotPlayer::RobotPlayer()
        : Player(names[Dice::GetRandomNumber(0, namesSize - 1)])
    {
    }

    RobotPlayer::RobotPlayer(const std::string& name)
        : Player(name)
    {
    }

    Character RobotPlayer::PickRoleDecision(const std::unordered_set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    void RobotPlayer::PlayRoleStep()
    {
    }
}