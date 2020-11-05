#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Point {
public :
	float x;
	float y;
	Point(float X, float Y) : x(X), y(Y) {};	// use the default copy constructor for now..
};

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height);

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);
  void ChangeDirection(Direction inpDir);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{3};          // changing this did not change the initial length..
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  Point IncrHead( float, float);	// just does math
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  std::vector<int> NextCell();
  bool WallOrBody(SDL_Point point);		// not used or defined currently..

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif