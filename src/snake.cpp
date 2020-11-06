#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

Point Snake::IncrHead( float x, float y) {
    switch (direction) {
    case Direction::kUp:
        y -= speed;
        break;

    case Direction::kDown:
        y += speed;
        break;

    case Direction::kLeft:
        x -= speed;
        break;

    case Direction::kRight:
        x += speed;
        break;
    }
    return Point(x, y);
}

void Snake::UpdateHead() {
  bool redirected = false;
  Point proj_head = IncrHead(head_x, head_y);
  int x = static_cast<int>(head_x);
  int y = static_cast<int>(head_y);
  int new_x = static_cast<int>(proj_head.x);
  int new_y = static_cast<int>(proj_head.y);

  switch (direction) {
  case Direction::kUp:
      if (proj_head.y < 0 || (y != new_y && SnakeCell(new_x, new_y)) ) {
          redirected = true;
          if (x <= 0 || SnakeCell(x - 1, y) )    // can't turn left coz you're at UL
              direction = Direction::kRight;
          else
              direction = Direction::kLeft;
      }
      break;

  case Direction::kDown:
      if (proj_head.y >= grid_height || (y != new_y && SnakeCell(new_x, new_y)) ) {
          redirected = true;
          if (x >= grid_width - 1 || SnakeCell(x + 1, y) ) // can't make a left turn coz you're at BR
              direction = Direction::kLeft;
          else
              direction = Direction::kRight;
      }
      break;

  case Direction::kRight:
      if (proj_head.x >= grid_width || x != new_x && SnakeCell(new_x, new_y) ) {
          redirected = true;
          if (y <= 0 || SnakeCell(x, y - 1) ) // can't make a left turn coz you're at UR
              direction = Direction::kDown;
          else
              direction = Direction::kUp;
      }
      break;

  case Direction::kLeft:
      if (proj_head.x <= 0 || x != new_x && SnakeCell(new_x, new_y)) {
          redirected = true;
          if (y >= grid_height - 1 || SnakeCell(x, y + 1) ) // can't make a left turn coz you're at BL
              direction = Direction::kUp;
          else
              direction = Direction::kDown;
      }
      break;
  }

  if (redirected) 
      proj_head = IncrHead( head_x , head_y );
   
  head_x = proj_head.x;
  head_y = proj_head.y;
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

Snake::Snake(int grid_width, int grid_height)
    : grid_width(grid_width),
    grid_height(grid_height),
    head_x(grid_width  / 2),
    head_y(grid_height / 2) {
// just to put in initial length
    // get direction, and, based on that, add points to the body
    int x_incr, y_incr;
    switch (direction) {
    case Direction::kUp :
        x_incr = 0;
        y_incr = -1;
        break;
    case Direction::kDown :
        x_incr = 0;
        y_incr = 1;
        break;
    case Direction::kLeft :
        x_incr = -1;
        y_incr = 0;
        break;
    case Direction::kRight :
        x_incr = 1;
        y_incr = 0;
    }
    for (auto count = 1; count < size; count++) {
        SDL_Point cell{
            static_cast<int>(head_x-(size-count)*x_incr),
            static_cast<int>(head_y-(size-count)*y_incr) };
        body.push_back(cell);
    }
}

std::vector<int> Snake::NextCell() {
    // given current direction and head coordinates, what is the next cell?
    std::vector<int> next_xy{ 0,0 };
    int x = static_cast<int>(head_x);
    int y = static_cast<int>(head_y);
    switch (direction) {
    case Direction::kUp:
        y -= 1;
        break;

    case Direction::kDown:
        y += 1;
        break;

    case Direction::kLeft:
        x -= 1;
        break;

    case Direction::kRight:
        x += 1;
        break;
    }
    next_xy[0] = x;
    next_xy[1] = y;
    return next_xy;
}

void Snake::ChangeDirection(Direction inpDir) {
    // uses the given input as a suggestion and looks at what happens before implement or rejecting
    Direction curDir = direction;
    direction = inpDir;
    std::vector<int> next_xy = NextCell();
    if (SnakeCell(next_xy[0], next_xy[1]))
        direction = curDir;     // revert
    else {
        // since you can't be wall and body :)
        if (next_xy[0] < 0 || next_xy[0] >= grid_width ||
            next_xy[1] < 0 || next_xy[1] >= grid_height)
            direction = curDir;
    }
}
