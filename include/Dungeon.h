#pragma once

#include "Position.h"
#include "Tile.h"
#include <vector>

class Dungeon {
public:
  Dungeon();

  void render(Position playerPosition);
  bool isWalkable(Position position) const;

private:
  std::vector<std::vector<Tile>> positions;

  void loadDungeon();
};
