#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

//int main() { // https://stackoverflow.com/questions/22741452/resolving-error-lnk2019-unresolved-external-symbol
// actually, even Lazy Foo has pointed this out. So, how does UD get away without it?
int main( int argc, char* argv[] ) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{240};
  constexpr std::size_t kScreenHeight{240};		// AC from 640
  constexpr std::size_t kNumXGrids{10};		// AC from 32
  constexpr std::size_t kNumYGrids{10};

  Renderer renderer(kScreenWidth, kScreenHeight, kNumXGrids, kNumYGrids);
  Controller controller;
  Game game(kNumXGrids, kNumYGrids);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}