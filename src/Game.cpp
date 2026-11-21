#include "Game.h"
#include "Direction.h"
#include <algorithm>
#include <iostream>
#include <random>

Game::Game() { spawnDemons(5); }

void Game::run() {
  while (running) {
    render();
    handleInput();
    update();
  }
}

void Game::render() {
  std::cout << "\033[2J\033[H";

  std::cout << R"(
╔═══════════════════════════════════════════════════╗
║                                                   ║
║  ▓   ▓  ▓▓▓   ▓▓▓  ▓▓▓▓▓ ▓   ▓ ▓▓▓▓  ▓   ▓ ▓▓▓▓▓  ║
║  ▓▓  ▓ ▓   ▓ ▓       ▓   ▓   ▓ ▓   ▓ ▓▓  ▓ ▓      ║
║  ▓ ▓ ▓ ▓   ▓ ▓       ▓   ▓   ▓ ▓▓▓▓  ▓ ▓ ▓ ▓▓▓▓   ║
║  ▓  ▓▓ ▓   ▓ ▓       ▓   ▓   ▓ ▓  ▓  ▓  ▓▓ ▓      ║
║  ▓   ▓  ▓▓▓   ▓▓▓    ▓    ▓▓▓  ▓   ▓ ▓   ▓ ▓▓▓▓▓  ║
║                                                   ║
╚═══════════════════════════════════════════════════╝
)";

  std::cout << "\nPlayer Health: " << player.health << "\n\n";

  std::vector<Position> demonPositions;

  for (unsigned int i = 0; i < demons.size(); i++) {
    demonPositions.push_back(demons[i].position);
  }

  dungeon.render(player.getPosition(), demonPositions);
}

void Game::handleInput() {
  std::cout
      << "Place your move: (WASD for walking / F for attacking / q to quit): ";
  char input;
  if (!(std::cin >> input)) {
    running = false;
    return;
  }

  switch (input) {
  case 'w':
    movePlayer(Direction::Up);
    break;
  case 's':
    movePlayer(Direction::Down);
    break;
  case 'a':
    movePlayer(Direction::Left);
    break;
  case 'd':
    movePlayer(Direction::Right);
    break;
  case 'q':
    running = false;
    break;
  }
}

void Game::movePlayer(Direction direction) {
  const Position destination = player.getNextPosition(direction);

  if (!dungeon.isWalkable(destination)) {
    std::cout << "Cannot move player there, try again..." << std::endl;
    return handleInput();
  }

  player.move(direction);
  moveDemons();
}

void Game::update() {}

void Game::spawnDemons(std::size_t count) {
  auto available = dungeon.getWalkablePositions();
  const Position start = player.getPosition();

  std::erase_if(available, [start](Position tile) {
    return tile.x == start.x && tile.y == start.y;
  });

  std::mt19937 random{std::random_device{}()};
  std::shuffle(available.begin(), available.end(), random);

  available.resize(std::min(count, available.size()));

  for (Position pos : available) {
    demons.push_back(pos);
  }
}

void Game::moveDemons() {
  const Position target = player.getPosition();

  static std::mt19937 rng{std::random_device{}()};
  std::bernoulli_distribution takesStep{0.5};

  for (Demon &demon : demons) {
    if (!takesStep(rng)) {
      continue;
    }

    const Position current = demon.getPosition();

    auto tryStep = [&](Direction direction, Position next) {
      if ((next.x == target.x && next.y == target.y) ||
          !dungeon.isWalkable(next)) {
        return false;
      }

      const bool occupied =
          std::any_of(demons.begin(), demons.end(), [next](const Demon &other) {
            const Position position = other.getPosition();
            return position.x == next.x && position.y == next.y;
          });

      if (occupied) {
        return false;
      }

      demon.move(direction);
      return true;
    };

    // Try moving horizontally toward the player first.
    if (current.x != target.x) {
      const bool moveRight = target.x > current.x;
      if (tryStep(moveRight ? Direction::Right : Direction::Left,
                  {current.x + (moveRight ? 1 : -1), current.y})) {
        continue;
      }
    }

    // Try vertically if the horizontal step was blocked or unnecessary.
    if (current.y != target.y) {
      const bool moveDown = target.y > current.y;
      tryStep(moveDown ? Direction::Down : Direction::Up,
              {current.x, current.y + (moveDown ? 1 : -1)});
    }
  }
}
