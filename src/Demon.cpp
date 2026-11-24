#include "Demon.h"

// Constructor
Demon::Demon(Position spawnPosition) { position = spawnPosition; }

// Public
Position Demon::getPosition() const { return position; };

Position Demon::getNextPosition(Direction direction) const {
  Position next = position;

  switch (direction) {
  case Direction::Up:
    --next.y;
    break;
  case Direction::Right:
    ++next.x;
    break;
  case Direction::Down:
    ++next.y;
    break;
  case Direction::Left:
    --next.x;
    break;
  }

  return next;
}

void Demon::move(Direction direction) {
  position = getNextPosition(direction);
};
