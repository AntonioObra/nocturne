#include "Game.h"
#include "Direction.h"
#include <iostream>

Game::Game() {}

void Game::run() {
  while (running) {
    render();
    handleInput();
    update();
  }
}

void Game::render() {
  std::cout << "\033[2J\033[H";
  dungeon.render(player.getPosition());
}

void Game::handleInput() {
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
