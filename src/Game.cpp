#include "Game.h"
#include <iostream>

Game::Game() {}

void Game::run() {
  while (running) {
    std::cout << "render" << std::endl;
    render();
    handleInput();
    update();
  }
}

void Game::render() {}

void Game::handleInput() {}

void Game::update() {}
