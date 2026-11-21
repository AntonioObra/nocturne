#pragma once

#include "Dungeon.h"

class Game {
public:
  Game();

  void run();

private:
  bool running = true;
  Dungeon dungeon;

  void render();
  void handleInput();
  void update();
};
