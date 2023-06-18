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

## Rules (pasted from [boardgamegeek](https://boardgamegeek.com/boardgame/478/citadels))

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

## Optional Kafka broker

````
mkdir -p zoo/data zoo/logs broker/data broker/secrets
sudo docker-compose -d up
````

## Badges

| Continuous Integration | master | unstable |
| ------------- |:-------------:|:-----:|
| GNU/Linux      | [![Build Status](https://travis-ci.org/RichardDally/Citadel.svg?branch=master)](https://travis-ci.org/RichardDally/Citadel) | [![Build Status](https://travis-ci.org/RichardDally/Citad\
el.svg?branch=unstable)](https://travis-ci.org/RichardDally/Citadel) |
| Windows      | [![Build status](https://ci.appveyor.com/api/projects/status/08pocyyslo6gpvkx/branch/master?svg=true)](https://ci.appveyor.com/project/RichardDally/citadel/branch/master) | [![Build sta\
tus](https://ci.appveyor.com/api/projects/status/08pocyyslo6gpvkx/branch/unstable?svg=true)](https://ci.appveyor.com/project/RichardDally/citadel/branch/unstable) |

| Codacy     | Coverity Scan    |
| ------------- |:-------------:|
| [![Codacy Badge](https://api.codacy.com/project/badge/Grade/e06bfa6a11ad40688153dcd55f4bf3e0)](https://www.codacy.com/app/RichardDally/Citadel?utm_source=github.com&amp;utm_medium=referral&amp;utm_con\
tent=RichardDally/Citadel&amp;utm_campaign=Badge_Grade) | <a href="https://scan.coverity.com/projects/richarddally-citadel"> <img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects\
/6253/badge.svg"/> </a> |
