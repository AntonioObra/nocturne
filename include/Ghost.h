#pragma once

#include "Direction.h"
#include "Position.h"

class Ghost {
public:
  Position position;

  Ghost(Position spawnPosition);

  void move(Direction direction);

  Position getPosition() const;
  Position getNextPosition(Direction direction) const;
};
