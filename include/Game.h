#pragma once

#include "Direction.h"
#include "Dungeon.h"
#include "Player.h"

class Game {
public:
  Game();

  void run();

private:
  bool running = true;

  Dungeon dungeon;
  Player player;

  void render();
  void handleInput();
  void update();
  void movePlayer(Direction direction);
};
