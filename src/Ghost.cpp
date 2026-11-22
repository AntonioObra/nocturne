#include "Ghost.h"

// Constructor
Ghost::Ghost(Position spawnPosition) { position = spawnPosition; }

// Public
Position Ghost::getPosition() const { return position; };

Position Ghost::getNextPosition(Direction direction) const {
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

void Ghost::move(Direction direction) {
  position = getNextPosition(direction);
};
