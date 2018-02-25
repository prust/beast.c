#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"

typedef struct {
  int x;
  int y;
} beast;

// forward-declare functions
void killBeast(beast* b);
bool isBeastDead(beast* b);
int toX(int ix);
int toY(int ix);
int toIx(int x, int y);
bool push(bool blocks[], beast beasts[], int dir_x, int dir_y, int pos_x, int pos_y);
int error(char* activity);

int block_w = 25;
int block_h = 25;
int block_density_pct = 30;
int player_x = 0;
int player_y = 0;

int num_blocks_w;
int num_blocks_h;
unsigned int last_move_time = 0;
int beast_speed = 500; // ms between moves
const int num_beasts = 5;

int main(int num_args, char* args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return error("initializing SDL");

  beast beasts[num_beasts];

  // starting positions (TODO: make these randomized)
  beasts[0].x = 10;
  beasts[0].y = 7;
  beasts[1].x = 27;
  beasts[1].y = 2;
  beasts[2].x = 31;
  beasts[2].y = 14;
  beasts[3].x = 15;
  beasts[3].y = 12;
  beasts[4].x = 8;
  beasts[4].y = 20;

  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) < 0)
    return error("getting display mode");

  srand(time(NULL));

  dm.w = 38 * block_w;
  dm.h = 28 * block_h;

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
  // if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0)
  //   return error("setting fullscreen");

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
            if (push(blocks, beasts, dir_x, dir_y, player_x, player_y)) {
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

    if (SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) < 0)
      return error("setting block color");

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

    if (SDL_SetRenderDrawColor(renderer, 140, 60, 140, 255) < 0)
      return error("setting player color");
    SDL_Rect player_rect = {
      .x = player_x * block_w,
      .y = player_y * block_h,
      .w = block_w,
      .h = block_h
    };
    if (SDL_RenderFillRect(renderer, &player_rect) < 0)
      return error("filling rect");

    if (SDL_SetRenderDrawColor(renderer, 140, 60, 60, 255) < 0)
      return error("setting beast color");
    for(int i = 0; i < num_beasts; ++i) {
      if (isBeastDead(&beasts[i]))
        continue;
      SDL_Rect beast_rect = {
        .x = beasts[i].x * block_w,
        .y = beasts[i].y * block_h,
        .w = block_w,
        .h = block_h
      };
      if (SDL_RenderFillRect(renderer, &beast_rect) < 0)
        return error("filling beast rect");
    }

    if (SDL_GetTicks() - last_move_time >= beast_speed) {
      for (int i = 0; i < num_beasts; ++i) {
        if (isBeastDead(&beasts[i]))
          continue;

        int x = beasts[i].x;
        int y = beasts[i].y;

        int dir_x = 0;
        int dir_y = 0;
        if (player_x < x)
          dir_x = -1;
        else if (player_x > x)
          dir_x = 1;

        if (player_y < y)
          dir_y = -1;
        else if (player_y > y)
          dir_y = 1;

        // try to move towards the player, if possible
        if (dir_x && dir_y && !blocks[toIx(x + dir_x, y + dir_y)]) {
          x += dir_x;
          y += dir_y;
        }
        else if (dir_x && !blocks[toIx(x + dir_x, y)]) {
          x += dir_x;
        }
        else if (dir_y && !blocks[toIx(x, y + dir_y)]) {
          y += dir_y;
        }
        // otherwise, try all other combinations of directions
        else if (!blocks[toIx(x + 1, y + 1)]) {
          x++;
          y++;
        }
        else if (!blocks[toIx(x + 1, y)]) {
          x++;
        }
        else if (!blocks[toIx(x, y + 1)]) {
          y++;
        }
        else if (!blocks[toIx(x + 1, y - 1)]) {
          x++;
          y--;
        }
        else if (!blocks[toIx(x - 1, y + 1)]) {
          x--;
          y++;
        }
        else if (!blocks[toIx(x - 1, y)]) {
          x--;
        }
        else if (!blocks[toIx(x, y - 1)]) {
          y--;
        }
        else if (!blocks[toIx(x - 1, y - 1)]) {
          x--;
          y--;
        }
        // if the beast is surrounded by blocks & has nowhere to move, it blows up
        else {
          killBeast(&beasts[i]);
        }

        beasts[i].x = x;
        beasts[i].y = y;
        if (x == player_x && y == player_y)
          is_gameover = true;
      }
      last_move_time = SDL_GetTicks();
    }
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

bool push(bool blocks[], beast beasts[], int dir_x, int dir_y, int pos_x, int pos_y) {
  int first_x = pos_x + dir_x;
  int first_y = pos_y + dir_y;
  if (first_x < 0 || first_x >= num_blocks_w || first_y < 0 || first_y >= num_blocks_h)
    return false;

  if (!blocks[toIx(first_x, first_y)])
    return true;

  int second_x = pos_x + dir_x*2;
  int second_y = pos_y + dir_y*2;
  if (second_x < 0 || second_x >= num_blocks_w || second_y < 0 || second_y >= num_blocks_h)
    return false;

  bool can_push;
  if (blocks[toIx(second_x, second_y)]) {
    can_push = push(blocks, beasts, dir_x, dir_y, pos_x + dir_x, pos_y + dir_y);
  }
  else {
    can_push = true;
    int third_x = pos_x + dir_x*3;
    int third_y = pos_y + dir_y*3;

    // only check for a brick on the other side & the possibility of squishing
    // if we're within bounds
    if (third_x >= 0 && third_x < num_blocks_w && third_y >= 0 && third_y < num_blocks_h) {
      // check to tell if you're squishing a beast
      for(int i = 0; i < num_beasts; ++i) {
        if (beasts[i].x == second_x && beasts[i].y == second_y) {

          // if there's a block on the other side, squish beast between blocks
          if (blocks[toIx(third_x, third_y)])
            killBeast(&beasts[i]);
          // disallow pushing block into beast if there's no block to squish against
          else
            return false;
        }
      }
    }
  }

  if (can_push) {
    blocks[toIx(first_x, first_y)] = false;
    blocks[toIx(second_x, second_y)] = true;
  }
  return can_push;
}

void killBeast(beast* b) {
  b->x = -1;
  b->y = -1;
}

bool isBeastDead(beast* b) {
  return b->x == -1 && b->y == -1;
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
