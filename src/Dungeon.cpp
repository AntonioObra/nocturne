#include "Dungeon.h"
#include <fstream>
#include <iostream>

Dungeon::Dungeon() { loadDungeon(); };

void Dungeon::render(Position playerPosition) {
  for (std::size_t y = 0; y < positions.size(); y++) {
    for (std::size_t x = 0; x < positions[y].size(); x++) {
      if (static_cast<int>(x) == playerPosition.x &&
          static_cast<int>(y) == playerPosition.y) {
        std::cout << "\033[1;32mP\033[m";
        continue;
      }

      Tile tile = positions[y][x];

      switch (tile) {
      case Tile::Wall:
        std::cout << "\033[1;34m#\033[m";
        break;
      case Tile::Floor:
        std::cout << "\033[1;37m.\033[m";
        break;
      case Tile::Door:
        std::cout << "\033[1;33m>\033[m";
        break;
      }
    }

    std::cout << "\n";
  }

  std::cout << "\n";
}

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

bool Dungeon::isWalkable(Position position) const {
  if (position.x < 0 || position.y < 0) {
    return false;
  }

  const auto y = static_cast<std::size_t>(position.y);
  const auto x = static_cast<std::size_t>(position.x);

  return y < positions.size() && x < positions[y].size() &&
         positions[y][x] == Tile::Floor;
}
