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

void Snake::IncrHead() {
    switch (direction) {
    case Direction::kUp:
        head_y -= speed;
        break;

    case Direction::kDown:
        head_y += speed;
        break;

    case Direction::kLeft:
        head_x -= speed;
        break;

    case Direction::kRight:
        head_x += speed;
        break;
    }
}

void Snake::UpdateHead() {
  bool redirected = false;
  IncrHead();
  int x = static_cast<int>(head_x);
  int y = static_cast<int>(head_y);

  switch (direction) {
  case Direction::kUp:
      if (head_y < 0) {
          head_y += speed;
          redirected = true;
          if (x <= 0)    // can't turn left coz you're at UL
              direction = Direction::kRight;
          else
              direction = Direction::kLeft;
      }
      break;

  case Direction::kDown:
      if (head_y >= grid_height) {
          head_y -= speed;
          redirected = true;
          if (x >= grid_width - 1) // can't make a left turn coz you're at BR
              direction = Direction::kLeft;
          else
              direction = Direction::kRight;
      }
      break;

  case Direction::kRight:
      if (head_x >= grid_width) {
          head_x -= speed;
          redirected = true;
          if (y <= 0 ) // can't make a left turn coz you're at UR
              direction = Direction::kDown;
          else
              direction = Direction::kUp;
      }
      break;

  case Direction::kLeft:
      if (head_x <= 0) {
          head_x += speed;
          redirected = true;
          if (y >= grid_height - 1 ) // can't make a left turn coz you're at BL
              direction = Direction::kUp;
          else
              direction = Direction::kDown;
      }
      break;
  }
  // violating DRY for now - will fix later :)
  if (redirected)
      IncrHead();
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
