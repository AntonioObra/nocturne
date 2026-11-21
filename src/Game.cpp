#include "Game.h"
#include "Direction.h"
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

  if (dungeon.isWalkable(destination)) {
    player.move(direction);
  }
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
