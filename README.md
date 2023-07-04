# Citadel - [Bruno Faidutti](http://faidutti.com/blog)'s boardgame coded in C++

Console text game (no user interface), play against AI or local humans (no network play).

## Usage

````
./Citadel --help

Usage:
  ./Citadel [OPTION...]

  -i, --interactive         Interactive game
  -a, --automated_game arg  Set AI players count (min 2, max 6) (default: 6)
  -h, --help                Print help
````

## Rules (from [boardgamegeek](https://boardgamegeek.com/boardgame/478/citadels))

In Citadels, players take on new roles each round to represent characters they hire in order to help them acquire gold and erect buildings. The game ends at the close of a round in which a player erect\
s his/her eighth building. Players then tally their points, and the player with the highest score wins.

Players start with a number of building cards in their hand; buildings come in five colors, with the purple buildings typically having a special ability and the other colored buildings providing a benef\
it when you play particular characters. At the start of each round, the player who was king the previous round discards one of the eight character cards at random, chooses one, then passes the cards to \
the next player, etc. until each player has secretly chosen a character. Each character has a special ability, and the usefulness of any character depends upon your situation, and that of your opponents\
. The characters then carry out their actions in numerical order: the assassin eliminating another character for the round, the thief stealing all gold from another character, the wizard swapping buildi\
ng cards with another player, the warlord optionally destroys a building in play, and so on.

On a turn, a player earns two or more gold (or draws two building cards then discards one), then optionally constructs one building (or up to three if playing the architect this round). Buildings cost g\
old equal to the number of symbols on them, and each building is worth a certain number of points. In addition to points from buildings, at the end of the game a player scores bonus points for having ei\
ght buildings or buildings of all five colors.

## Optional Dockerized Kafka cluster

````
mkdir -p zoo/data zoo/logs broker/data broker/secrets
sudo docker-compose -d up
````
