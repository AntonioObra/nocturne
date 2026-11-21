#include "Game.h"
#include <iostream>

Game::Game() {}

void Game::run() {
  while (running) {
    render();
    handleInput();
    update();
  }
}

void Game::render() { dungeon.render(); }

void Game::handleInput() {
  std::string input;
  std::cin >> input;
}

void Game::update() {}
