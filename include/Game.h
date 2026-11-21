#pragma once

#include "Demon.h"
#include "Direction.h"
#include "Dungeon.h"
#include "Player.h"
#include <vector>

class Game {
public:
  Game();

  void run();

private:
  bool running = true;

  Dungeon dungeon;
  Player player;
  std::vector<Demon> demons;

  void render();
  void handleInput();
  void update();
  void movePlayer(Direction direction);
  void moveDemon();
  void spawnDemons(std::size_t count);
};
