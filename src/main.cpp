#include "GameData.h"
#include "Boardgame.h"

int main()
{
    Citadel::Boardgame b(Citadel::Edition::REGULAR);
    b.StartGame();
    return 0;
}