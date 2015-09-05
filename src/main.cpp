#include "GameData.h"
#include "Boardgame.h"

int main()
{
    Citadel::Boardgame boardgame(Citadel::Edition::REGULAR);
    boardgame.StartGame();
    return 0;
}