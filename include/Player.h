#pragma once

#include "Direction.h"
#include "Position.h"

class Player {
public:
  int health = 100;

  void attack();
  void move(Direction direction);

  Position getPosition() const;
  Position getNextPosition(Direction direction) const;

private:
  Position position{1, 1};
};
