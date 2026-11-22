#pragma once

#include "Position.h"
#include "Tile.h"
#include <vector>

class Dungeon {
public:
  Dungeon();

  void render(Position playerPosition, const std::vector<Position> &demons,
              const std::vector<Position> &ghosts);
  bool isWalkable(Position position) const;
  std::vector<Position> getWalkablePositions() const;

private:
  std::vector<std::vector<Tile>> positions;

  void loadDungeon();
};
