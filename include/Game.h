#pragma once

class Game {
public:
  Game();

  void run();

private:
  bool running = true;

  void render();
  void handleInput();
  void update();
};
