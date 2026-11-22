#include "Dungeon.h"
#include <algorithm>
#include <fstream>
#include <iostream>

// Constructor
Dungeon::Dungeon() { loadDungeon(); };

// Public
void Dungeon::render(Position playerPosition,
                     const std::vector<Position> &demons,
                     const std::vector<Position> &ghosts) {
  for (std::size_t y = 0; y < positions.size(); y++) {
    for (std::size_t x = 0; x < positions[y].size(); x++) {
      // Player
      if (static_cast<int>(x) == playerPosition.x &&
          static_cast<int>(y) == playerPosition.y) {
        std::cout << "🧙";
        continue;
      }

      // Demons
      const bool hasDemon =
          std::any_of(demons.begin(), demons.end(), [x, y](Position demon) {
            return demon.x == static_cast<int>(x) &&
                   demon.y == static_cast<int>(y);
          });

      if (hasDemon) {
        std::cout << "👹";
        continue;
      }

      // Ghosts
      const bool hasGhost =
          std::any_of(ghosts.begin(), ghosts.end(), [x, y](Position ghost) {
            return ghost.x == static_cast<int>(x) &&
                   ghost.y == static_cast<int>(y);
          });

      if (hasGhost) {
        std::cout << "👻";
        continue;
      }

      // Normal tiles
      Tile tile = positions[y][x];

      // ? Add space and double ## instead of # because emojies take 2 spaces
      // instead of 1
      switch (tile) {
      case Tile::Wall:
        std::cout << "\033[1;34m##\033[m";
        break;
      case Tile::Floor:
        std::cout << "\033[1;37m. \033[m";
        break;
      case Tile::Door:
        std::cout << "\033[1;33m> \033[m";
        break;
      }
    }

    std::cout << "\n";
  }

  std::cout << "\n";
}

bool Dungeon::isWalkable(Position position) const {
  if (position.x < 0 || position.y < 0) {
    return false;
  }

  const auto y = static_cast<std::size_t>(position.y);
  const auto x = static_cast<std::size_t>(position.x);

  return y < positions.size() && x < positions[y].size() &&
         positions[y][x] == Tile::Floor;
}

std::vector<Position> Dungeon::getWalkablePositions() const {
  std::vector<Position> walkable;

  for (std::size_t y = 0; y < positions.size(); y++) {
    for (std::size_t x = 0; x < positions[y].size(); x++) {
      Position tile{static_cast<int>(x), static_cast<int>(y)};

      if (isWalkable(tile)) {
        walkable.push_back(tile);
      }
    }
  }

  return walkable;
}

// Private
void Dungeon::loadDungeon() {
  std::ifstream file("assets/maps/dungeon_01.txt");
  std::string str;

  while (std::getline(file, str)) {
    std::vector<char> letters(str.begin(), str.end());

    std::vector<Tile> tiles;
    for (char letter : letters) {
      switch (letter) {
      case '#':
        tiles.push_back(Tile::Wall);
        break;
      case '.':
        tiles.push_back(Tile::Floor);
        break;
      case '>':
        tiles.push_back(Tile::Door);
        break;
      }
    }

    Dungeon::positions.push_back(tiles);
  }
}
