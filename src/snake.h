#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <algorithm>
#include "SDL.h"

#define INITIAL_LENGTH 3
#define SKIN_DEPTH 2	// number of pixels to take off from block width on each side

class Food {
public:
	SDL_Point location;
	std::vector<int> RGB{ 0xFF, 0xCC, 0x00 };
};

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
  const std::vector<std::vector<int>>& GetColors() const { return colors;  }
  void ScaleSpeed(float factor) { speed *= factor; }
  int GetSize() const { return size; }
  bool SizeInBounds(int lower, int higher) const { return size > lower && size < higher; }
  void Feed( Food snack) { food.RGB.assign( snack.RGB.begin(), snack.RGB.end() );  } // we only care about color.. sorry :)

 private:
  bool alive{true};
  int size{INITIAL_LENGTH};          // changing this did not change the initial length..
  float speed{0.1f};
  float head_x;	// AC moved to private
  float head_y;
  std::vector<SDL_Point> body;		// surprise, [0] is the tail.. so when you push_back(), that's the neck..
  std::vector<std::vector<int>> colors;
  Food food;	// the snake can see the food -- for now :) in a 2D world, you don't have LoSight if it's obscured by body..:)
  // is this the best way though?.. think about it..

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