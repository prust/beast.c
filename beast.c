#include "include/SDL.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

// forward-declare functions
int toX(int ix);
int toY(int ix);
int toIx(int x, int y);
bool push(bool blocks[], int len, int dir_x, int dir_y, int pos_x, int pos_y);
int error(char* activity);

int block_w = 15;
int block_h = 20;
int block_density_pct = 40;
int player_x = 0;
int player_y = 0;

int num_blocks_w;
int num_blocks_h;

int main(int num_args, char* args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return error("initializing SDL");

  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) < 0)
    return error("getting display mode");

  srand(time(NULL));

  num_blocks_w = dm.w / block_w;
  num_blocks_h = dm.h / block_h;
  int num_possible_blocks = num_blocks_w * num_blocks_h;

  bool blocks[num_possible_blocks];
  for (int i = 0; i < num_possible_blocks; i++)
    blocks[i] = (rand() % 100) < block_density_pct;

  SDL_Window *window;
  window = SDL_CreateWindow("Beast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, SDL_WINDOW_RESIZABLE);
  if (!window)
    return error("creating window");
  if (SDL_ShowCursor(SDL_DISABLE) < 0)
    return error("hiding cursor");
  if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0)
    return error("setting fullscreen");

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    return error("creating renderer");

  bool is_gameover = false;
  int dir_x = 0;
  int dir_y = 0;

  while (!is_gameover) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      switch(evt.type) {
        case SDL_QUIT:
          is_gameover = true;
          break;
        case SDL_KEYDOWN:
          dir_x = 0;
          dir_y = 0;
          switch (evt.key.keysym.sym) {
            case SDLK_ESCAPE:
              is_gameover = true;
              break;
            case SDLK_LEFT:
              dir_x = -1;
              break;
            case SDLK_RIGHT:
              dir_x = 1;
              break;
            case SDLK_UP:
              dir_y = -1;
              break;
            case SDLK_DOWN:
              dir_y = 1;
              break;
          }

          if (dir_x != 0 || dir_y != 0) {
            if (push(blocks, num_possible_blocks, dir_x, dir_y, player_x, player_y)) {
              player_x += dir_x;
              player_y += dir_y;
            }
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
    SDL_Rect player_rect = {
      .x = player_x * block_w,
      .y = player_y * block_h,
      .w = block_w,
      .h = block_h
    };
    if (SDL_RenderFillRect(renderer, &player_rect) < 0)
      return error("filling rect");

    for (int i = 0; i < num_possible_blocks; i++) {
      if (blocks[i]) {
        SDL_Rect r = {
          .x = toX(i) * block_w,
          .y = toY(i) * block_h,
          .w = block_w,
          .h = block_h
        };
        if (SDL_RenderFillRect(renderer, &r) < 0)
          return error("drawing block");
      }
    }
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

bool push(bool blocks[], int len, int dir_x, int dir_y, int pos_x, int pos_y) {
  int first_ix = toIx(pos_x + dir_x, pos_y + dir_y);
  int second_ix = toIx(pos_x + dir_x*2, pos_y + dir_y*2);
  if (blocks[first_ix]) {
    if (!blocks[second_ix]) {
      blocks[first_ix] = false;
      blocks[second_ix] = true;
      return true;
    }
    else {
      return false; // can't push if there are two blocks stacked
    }
  }
  else {
    return true;
  }
}

int toX(int ix) {
  return ix % num_blocks_w;
}
int toY(int ix) {
  return ix / num_blocks_w;
}
int toIx(int x, int y) {
  return x + y * num_blocks_w;
}

int error(char* activity) {
  printf("%s failed: %s\n", activity, SDL_GetError());
  SDL_Quit();
  return -1;
}
