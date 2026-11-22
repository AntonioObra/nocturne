#pragma once

#include "Demon.h"
#include "Direction.h"
#include "Dungeon.h"
#include "Ghost.h"
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
  std::vector<Ghost> ghosts;

  void render();
  void handleInput();
  void update();

  // Player
  void movePlayer(Direction direction);

  // Demons
  void spawnDemons(std::size_t count);
  void moveDemons();

  // Ghosts
  void spawnGhosts(std::size_t count);
  void moveGhosts();
};
