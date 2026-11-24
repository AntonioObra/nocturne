#pragma once

#include "Direction.h"
#include "Position.h"

class Demon {
public:
  Position position;
  int health = 100;
  int damage = 20;

  Demon(Position spawnPosition);

  void move(Direction direction);

  Position getPosition() const;
  Position getNextPosition(Direction direction) const;
};
