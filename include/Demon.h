#pragma once

#include "Direction.h"
#include "Position.h"

class Demon {
public:
  Position position;
  int health = 100;

  Demon(Position aposition) { position = aposition; }

  void attack();
  void move(Direction direction);

  Position getPosition() const;
  Position getNextPosition(Direction direction) const;
};
