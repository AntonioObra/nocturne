#include "Player.h"

void Player::attack() {};

Position Player::getPosition() const { return position; };

Position Player::getNextPosition(Direction direction) const {
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

void Player::move(Direction direction) {
  position = getNextPosition(direction);
};
