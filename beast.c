#include "include/SDL.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

// forward-declaring functions
int error(char* activity);

int block_w = 15;
int block_h = 20;
float block_density = 0.4;


int main(int num_args, char* args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return error("initializing SDL");

  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) < 0)
    return error("getting display mode");

  srand(time(NULL));

  // use density to determine # of blocks & allocate memory for them
  // in one shot on the stack
  // this is more efficient than walking through each x/y block position
  // & deciding randomly whether to have a block there
  int num_blocks_w = dm.w / block_w;
  int num_blocks_h = dm.h / block_h;
  int num_possible_blocks = num_blocks_w * num_blocks_h;
  int num_blocks = num_possible_blocks * block_density;

  SDL_Rect blocks[num_blocks];

  for (int i = 0; i < num_blocks; i++) {
    blocks[i].x = (rand() % num_blocks_w) * block_w;
    blocks[i].y = (rand() % num_blocks_h) * block_h;
    blocks[i].w = block_w;
    blocks[i].h = block_h;
  }

  SDL_Window *window;
  window = SDL_CreateWindow("Beast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, SDL_WINDOW_RESIZABLE);
  if (!window)
    return error("creating window");
  if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0)
    return error("setting fullscreen");

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    return error("creating renderer");

  SDL_Rect player;
  player.x = 0;
  player.y = 0;
  player.w = block_w;
  player.h = block_h;

  bool is_gameover = false;

  while (!is_gameover) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      switch(evt.type) {
        case SDL_QUIT:
          is_gameover = true;
          break;
        case SDL_KEYDOWN:
          switch (evt.key.keysym.sym) {
            case SDLK_ESCAPE:
              is_gameover = true;
              break;
            case SDLK_LEFT:
              player.x -= player.w;
              break;
            case SDLK_RIGHT:
              player.x += player.w;
              break;
            case SDLK_UP:
              player.y -= player.h;
              break;
            case SDLK_DOWN:
              player.y += player.h;
              break;
        }
        break;
      }
    }

    // set BG color
    if (SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255) < 0)
      return error("setting bg color");
    if (SDL_RenderClear(renderer) < 0)
      return error("clearing renderer");

    // set block color
    if (SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) < 0)
      return error("setting block color");
    if (SDL_RenderFillRect(renderer, &player) < 0)
      return error("filling rect");

    for (int i = 0; i < num_blocks; i++)
      if (SDL_RenderFillRect(renderer, &blocks[i]) < 0)
        return error("drawing block");
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

int error(char* activity) {
  printf("%s failed: %s\n", activity, SDL_GetError());
  SDL_Quit();
  return -1;
}
