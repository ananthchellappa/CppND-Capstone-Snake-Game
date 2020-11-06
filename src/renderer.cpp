#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  RenderBody(snake, block);

  // Render snake's head
  block.x = static_cast<int>(snake.GetHead().x) * block.w;
  block.y = static_cast<int>(snake.GetHead().y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

Renderer::Direction Renderer::Oriented(int x1, int y1, int x2, int y2)
{
    // x1,y1 are predecessor - what x2,y2 are looking at..
    if (y1 < y2)
        return Direction::kUp;
    else
        if (y1 > y2)
            return Direction::kDown;
        else
            if (x1 < x2)
                return Direction::kLeft;
            else
                return Direction::kRight;
    return Direction::kDown; // should never reach here..
}

Renderer::Direction Renderer::Oriented(SDL_Point p1, SDL_Point p2)
{   // p1 is predecessor
    if (p1.y < p2.y)
        return Direction::kUp;
    else
        if (p1.y > p2.y)
            return Direction::kDown;
        else
            if (p1.x < p2.x)
                return Direction::kLeft;
            else
                return Direction::kRight;
    return Direction::kDown; // should never reach here..

}

void Renderer::RenderBlock(Direction dir, int x, int y, SDL_Rect& block)
{   // draw the appropriate rectangle with (sadly) in-place manipulation of block, which
    // should be left as it was, in the end (in terms of w and h, not x,y )
    switch (dir) {
    case Direction::kUp :
        block.x = x * block.w + 1;
        block.y = y * block.h - 1;
        block.w -= 2;
        SDL_RenderFillRect(sdl_renderer, &block);
        block.w += 2;
        break;
    case Direction::kDown :
        block.x = x * block.w + 1;
        block.y = y * block.h + 1;
        block.w -= 2;
        SDL_RenderFillRect(sdl_renderer, &block);
        block.w += 2;
        break;
    case Direction::kLeft :
        block.x = x * block.w - 2;
        block.y = y * block.h + 1;
        block.h -= 2;
        block.w++;
        SDL_RenderFillRect(sdl_renderer, &block);
        block.h += 2;
        block.w--;
        break;
    case Direction::kRight :
        block.x = x * block.w + 1;
        block.y = y * block.h + 1;
        block.h -= 2;
        //block.w++;
        SDL_RenderFillRect(sdl_renderer, &block);
        block.h += 2;
        //block.w--;
        break;

    }
}

void Renderer::RenderBody(Snake const snake, SDL_Rect &block)
{
    Direction orientation;
    int x = static_cast<int>(snake.GetHead().x);
    int y = static_cast<int>(snake.GetHead().y);
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    orientation = Oriented(x, y, snake.body.back().x, snake.body.back().y  );
    RenderBlock(orientation, snake.body.back().x, snake.body.back().y, block);
    for (auto point = snake.body.rbegin() +1; point != snake.body.rend(); point++) {
        orientation = Oriented( *(point - 1), *point);
        RenderBlock(orientation, point->x, point->y, block);
    }
}

