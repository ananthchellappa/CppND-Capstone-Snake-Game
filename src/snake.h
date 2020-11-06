#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

#define INITIAL_LENGTH 3

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

  bool IsAlive() const { return alive; }
  Point GetHead() const { return Point(head_x, head_y); }
  const std::vector<SDL_Point>& GetBody() const { return body; }
  void ScaleSpeed(float factor) { speed *= factor; }
  int GetSize() const { return size; }
  bool SizeInBounds(int lower, int higher) const { return size > lower && size < higher; }

 private:
  bool alive{true};
  int size{INITIAL_LENGTH};          // changing this did not change the initial length..
  float speed{0.1f};
  float head_x;	// AC moved to private
  float head_y;
  std::vector<SDL_Point> body;

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