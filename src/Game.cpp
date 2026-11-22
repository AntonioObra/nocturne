#include "Game.h"
#include "Direction.h"
#include <algorithm>
#include <iostream>
#include <random>

// Constructor
Game::Game() {
  spawnDemons(2);
  spawnGhosts(5);
}

// Public
void Game::run() {
  while (running) {
    render();
    handleInput();
    update();
  }
}

// Private
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
  std::vector<Position> ghostPositions;

  for (unsigned int i = 0; i < demons.size(); i++) {
    demonPositions.push_back(demons[i].position);
  }

  for (unsigned int i = 0; i < ghosts.size(); i++) {
    ghostPositions.push_back(ghosts[i].position);
  }

  dungeon.render(player.getPosition(), demonPositions, ghostPositions);
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
  moveGhosts();
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
    demons.push_back(Demon{pos});
  }
}

void Game::moveDemons() {
  const Position playerPosition = player.getPosition();

  static std::mt19937 rng{std::random_device{}()};
  std::bernoulli_distribution takesStep{0.5};

  for (Demon &demon : demons) {
    if (!takesStep(rng)) {
      continue;
    }

    const Position currentDemonPosition = demon.getPosition();

    // Lambda
    // TO KNOW: a small function you define right where you need it
    //
    // Check the new direction and position and move the demon if its allowed
    // [&] part lets it use surrounding local variables like demon and
    // playerPosition by reference
    // direction = new direction
    // nextPosition = new possible position of demon
    auto tryStep = [&](Direction direction, Position nextPosition) {
      // If the new position of current Demon is the players current Position
      // dont walk on it
      // TODO: attack player instead
      if ((nextPosition.x == playerPosition.x &&
           nextPosition.y == playerPosition.y) ||
          !dungeon.isWalkable(nextPosition)) {
        return false;
      }

      // * Check if any of the other demons are on the new position of the
      // current demon
      // ? Here we are once again using lambda function and just
      // passing it nextPosition to use const Demon &other means the current
      // demon in a loop
      const bool positionOccupiedByOtherDemon = std::any_of(
          demons.begin(), demons.end(), [nextPosition](const Demon &other) {
            const Position position = other.getPosition();
            return position.x == nextPosition.x && position.y == nextPosition.y;
          });

      if (positionOccupiedByOtherDemon) {
        return false;
      }

      // Check if position ocupied by a ghost
      const bool positionOccupiedByGhost = std::any_of(
          ghosts.begin(), ghosts.end(), [nextPosition](const Ghost &ghost) {
            const Position position = ghost.getPosition();
            return position.x == nextPosition.x && position.y == nextPosition.y;
          });

      if (positionOccupiedByGhost) {
        return false;
      }

      // * Move demon, the new position is free
      demon.move(direction);
      return true;
    };

    // Try moving horizontally toward the player first.
    if (currentDemonPosition.x != playerPosition.x) {
      const bool moveRight = playerPosition.x > currentDemonPosition.x;
      if (tryStep(moveRight ? Direction::Right : Direction::Left,
                  {currentDemonPosition.x + (moveRight ? 1 : -1),
                   currentDemonPosition.y})) {
        continue;
      }
    }

    // Try vertically if the horizontal step was blocked or unnecessary.
    if (currentDemonPosition.y != playerPosition.y) {
      const bool moveDown = playerPosition.y > currentDemonPosition.y;
      tryStep(moveDown ? Direction::Down : Direction::Up,
              {currentDemonPosition.x,
               currentDemonPosition.y + (moveDown ? 1 : -1)});
    }
  }
}

void Game::spawnGhosts(std::size_t count) {
  auto available = dungeon.getWalkablePositions();
  const Position start = player.getPosition();

  std::erase_if(available, [start](Position tile) {
    return tile.x == start.x && tile.y == start.y;
  });

  std::mt19937 random{std::random_device{}()};
  std::shuffle(available.begin(), available.end(), random);

  available.resize(std::min(count, available.size()));

  for (Position pos : available) {
    ghosts.push_back(Ghost{pos});
  }
}

void Game::moveGhosts() {
  const Position playerPosition = player.getPosition();

  for (Ghost &ghost : ghosts) {

    const Position currentGhostPosition = ghost.getPosition();

    // Check the new direction and position and move the ghost if its allowed
    // direction = new direction
    // nextPosition = new possible position of demon
    auto tryStep = [&](Direction direction, Position nextPosition) {
      // If the new position of current Ghost is the players current Position
      // dont walk on it
      // TODO: attack player instead
      if (nextPosition.x == playerPosition.x &&
          nextPosition.y == playerPosition.y) {
        return false;
      }

      // * Check if any of the demons are on the new position of the
      // current ghost
      const bool positionOccupiedByDemon = std::any_of(
          demons.begin(), demons.end(), [nextPosition](const Demon &other) {
            const Position position = other.getPosition();
            return position.x == nextPosition.x && position.y == nextPosition.y;
          });

      if (positionOccupiedByDemon) {
        return false;
      }

      // Check if position ocupied by a ghost
      const bool positionOccupiedByGhost = std::any_of(
          ghosts.begin(), ghosts.end(), [nextPosition](const Ghost &ghost) {
            const Position position = ghost.getPosition();
            return position.x == nextPosition.x && position.y == nextPosition.y;
          });

      if (positionOccupiedByGhost) {
        return false;
      }

      // * Move demon, the new position is free
      ghost.move(direction);
      return true;
    };

    // Try moving horizontally toward the player first.
    if (currentGhostPosition.x != playerPosition.x) {
      const bool moveRight = playerPosition.x > currentGhostPosition.x;
      if (tryStep(moveRight ? Direction::Right : Direction::Left,
                  {currentGhostPosition.x + (moveRight ? 1 : -1),
                   currentGhostPosition.y})) {
        continue;
      }
    }

    // Try vertically if the horizontal step was blocked or unnecessary.
    if (currentGhostPosition.y != playerPosition.y) {
      const bool moveDown = playerPosition.y > currentGhostPosition.y;
      tryStep(moveDown ? Direction::Down : Direction::Up,
              {currentGhostPosition.x,
               currentGhostPosition.y + (moveDown ? 1 : -1)});
    }
  }
}
