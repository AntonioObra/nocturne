#pragma once

#include "Tile.h"
#include <vector>

class Dungeon {
public:
  std::vector<std::vector<Tile>> positions;

  Dungeon();

  void render();

private:
  void loadDungeon();
};
