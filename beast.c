#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"

typedef unsigned char byte;

#define DELETED 0x1
#define BLOCK   0x2
#define BEAST   0x4
#define PLAYER  0x8
#define STATIC 0x10

typedef struct {
  byte flags;
  int x;
  int y;
} entity;

// forward-declare functions
int findAvailPos(entity* grid[]);
void move(entity* ent, entity* grid[], int x, int y);
int to_x(int ix);
int to_y(int ix);
int to_pos(int x, int y);
bool push(entity* grid[], int dir_x, int dir_y, int pos_x, int pos_y);
void error(char* activity);

int block_w = 25;
int block_h = 25;
int block_density_pct = 30;
entity player = {
  .flags = PLAYER,
  .x = 1,
  .y = 1
};

int num_blocks_w = 40;
int num_blocks_h = 30;
int grid_len;

unsigned int last_move_time = 0;
int beast_speed = 500; // ms between moves
const int num_beasts = 5;

int main(int num_args, char* args[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    error("initializing SDL");

  srand(time(NULL));

  // SDL_DisplayMode dm; // gets resolution as struct w/ `w` and `h`
  // if (SDL_GetDesktopDisplayMode(0, &dm) < 0)
  //   error("getting display mode");

  grid_len = num_blocks_w * num_blocks_h;
  entity* grid[grid_len];
  for (int i = 0; i < grid_len; ++i)
    grid[i] = NULL;

  grid[to_pos(player.x, player.y)] = &player;

  int num_static_blocks = num_blocks_w * 2 + (num_blocks_h - 2) * 2 + 10;
  entity static_blocks[num_static_blocks];
  int ix = 0;
  for (int x = 0; x < num_blocks_w; ++x) {
    // top row
    static_blocks[ix].flags = (BLOCK | STATIC);
    static_blocks[ix].x = x;
    static_blocks[ix].y = 0;
    grid[to_pos(x, 0)] = &static_blocks[ix];
    ix++;

    // bottom row
    static_blocks[ix].flags = (BLOCK | STATIC);
    static_blocks[ix].x = x;
    static_blocks[ix].y = num_blocks_h - 1;
    grid[to_pos(x, num_blocks_h - 1)] = &static_blocks[ix];
    ix++;
  }

  for (int y = 1; y < num_blocks_h - 1; ++y) {
    // left row
    static_blocks[ix].flags = (BLOCK | STATIC);
    static_blocks[ix].x = 0;
    static_blocks[ix].y = y;
    grid[to_pos(0, y)] = &static_blocks[ix];
    ix++;

    // right row
    static_blocks[ix].flags = (BLOCK | STATIC);
    static_blocks[ix].x = num_blocks_w - 1;
    static_blocks[ix].y = y;
    grid[to_pos(num_blocks_w - 1, y)] = &static_blocks[ix];
    ix++;
  }

  // additional 10 static blocks in the playing field
  for (int i = 0; i < 10; ++i) {
    int pos = findAvailPos(grid);
    static_blocks[ix].flags = (BLOCK | STATIC);
    static_blocks[ix].x = to_x(pos);
    static_blocks[ix].y = to_y(pos);
    grid[pos] = &static_blocks[ix];
    ix++;
  }

  // DRY violation: consolidate below into place_entities()
  int num_blocks = grid_len * block_density_pct / 100;
  entity blocks[num_blocks];
  for (int i = 0; i < num_blocks; ++i) {
    int pos = findAvailPos(grid);
    blocks[i].flags = BLOCK;
    blocks[i].x = to_x(pos);
    blocks[i].y = to_y(pos);
    grid[pos] = &blocks[i];
  }

  entity beasts[num_beasts];
  for (int i = 0; i < num_beasts; ++i) {
    int pos = findAvailPos(grid);
    beasts[i].flags = BEAST;
    beasts[i].x = to_x(pos);
    beasts[i].y = to_y(pos);
    grid[pos] = &beasts[i];
  }

  SDL_Window *window;
  window = SDL_CreateWindow("Beast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, num_blocks_w * block_w, num_blocks_h * block_h, SDL_WINDOW_RESIZABLE);
  if (!window)
    error("creating window");
  if (SDL_ShowCursor(SDL_DISABLE) < 0)
    error("hiding cursor");
  // if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0)
  //   error("setting fullscreen");

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    error("creating renderer");

  bool is_gameover = false;
  int dir_x = 0;
  int dir_y = 0;
  while (!is_gameover) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    bool is_spacebar_pressed = state[SDL_SCANCODE_SPACE];

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

          if (dir_x || dir_y) {
            if (push(grid, dir_x, dir_y, player.x, player.y)) {
              int orig_x = player.x;
              int orig_y = player.y;
              move(&player, grid, player.x + dir_x, player.y + dir_y);
              if (is_spacebar_pressed) {
                entity* ent_behind = grid[to_pos(orig_x - dir_x, orig_y - dir_y)];
                if (ent_behind && (ent_behind->flags & BLOCK) && !(ent_behind->flags & STATIC))
                  move(ent_behind, grid, orig_x, orig_y);
              }
            }
          }
        break;
      }
    }

    // set BG color
    if (SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255) < 0)
      error("setting bg color");
    if (SDL_RenderClear(renderer) < 0)
      error("clearing renderer");

    if (SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) < 0)
      error("setting block color");

    for (int i = 0; i < num_blocks; ++i) {
      SDL_Rect r = {
        .x = blocks[i].x * block_w,
        .y = blocks[i].y * block_h,
        .w = block_w,
        .h = block_h
      };
      if (SDL_RenderFillRect(renderer, &r) < 0)
        error("drawing block");
    }

    if (SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255) < 0)
      error("setting block color");

    for (int i = 0; i < num_static_blocks; ++i) {
      SDL_Rect r = {
        .x = static_blocks[i].x * block_w,
        .y = static_blocks[i].y * block_h,
        .w = block_w,
        .h = block_h
      };
      if (SDL_RenderFillRect(renderer, &r) < 0)
        error("drawing block");
    }

    if (SDL_SetRenderDrawColor(renderer, 140, 60, 140, 255) < 0)
      error("setting player color");
    SDL_Rect player_rect = {
      .x = player.x * block_w,
      .y = player.y * block_h,
      .w = block_w,
      .h = block_h
    };
    if (SDL_RenderFillRect(renderer, &player_rect) < 0)
      error("filling rect");

    if (SDL_SetRenderDrawColor(renderer, 140, 60, 60, 255) < 0)
      error("setting beast color");
    for (int i = 0; i < num_beasts; ++i) {
      if (beasts[i].flags & DELETED)
        continue;

      SDL_Rect beast_rect = {
        .x = beasts[i].x * block_w,
        .y = beasts[i].y * block_h,
        .w = block_w,
        .h = block_h
      };
      if (SDL_RenderFillRect(renderer, &beast_rect) < 0)
        error("filling beast rect");
    }

    if (SDL_GetTicks() - last_move_time >= beast_speed) {
      for (int i = 0; i < num_beasts; ++i) {
        if (beasts[i].flags & DELETED)
          continue;

        int x = beasts[i].x;
        int y = beasts[i].y;

        int dir_x = 0;
        int dir_y = 0;
        if (player.x < x)
          dir_x = -1;
        else if (player.x > x)
          dir_x = 1;

        if (player.y < y)
          dir_y = -1;
        else if (player.y > y)
          dir_y = 1;

        bool found_direction = true;

        // the beast will "get" the player on this move
        if (abs(player.x - x) <= 1 && abs(player.y - y) <= 1) {
          is_gameover = true;
          x = player.x;
          y = player.y;
        }
        // try to move towards the player, if possible
        else if (dir_x && dir_y && !grid[to_pos(x + dir_x, y + dir_y)]) {
          x += dir_x;
          y += dir_y;
        }
        else if (dir_x && !grid[to_pos(x + dir_x, y)]) {
          x += dir_x;
        }
        else if (dir_y && !grid[to_pos(x, y + dir_y)]) {
          y += dir_y;
        }
        else {
          // try all other combinations of directions
          found_direction = false;
          for (int mv_x = -1; mv_x <= 1; ++mv_x) {
            if (!found_direction) {
              for (int mv_y = -1; mv_y <= 1; ++mv_y) {
                if (!mv_x && !mv_y)
                  continue; // 0,0 isn't a real move

                if (!grid[to_pos(x + mv_x, y + mv_y)]) {
                  x = x + mv_x;
                  y = y + mv_y;
                  found_direction = true;
                  break;
                }
              }
            }
          }
        }

        // if the beast is surrounded by blocks & has nowhere to move, it blows up
        if (!found_direction)
          beasts[i].flags |= DELETED; // turn deleted bit on
        else
          move(&beasts[i], grid, x, y);
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

bool push(entity* grid[], int dir_x, int dir_y, int pos_x, int pos_y) {
  entity* first_ent = grid[to_pos(pos_x + dir_x, pos_y + dir_y)];
  if (!first_ent)
    return true;
  if (first_ent->flags & STATIC)
    return false;

  int second_x = pos_x + dir_x*2;
  int second_y = pos_y + dir_y*2;
  bool can_push;
  entity* second_ent = grid[to_pos(second_x, second_y)];
  if (!second_ent) {
    can_push = true;
  }
  else if (second_ent->flags & BLOCK) {
    can_push = push(grid, dir_x, dir_y, pos_x + dir_x, pos_y + dir_y);
  }
  else if (second_ent->flags & PLAYER) {
    can_push = false;
  }
  else if (second_ent->flags & BEAST) {
    // if there's a block on the other side, squish beast between blocks
    entity* third_ent = grid[to_pos(pos_x + dir_x*3, pos_y + dir_y*3)];
    if (third_ent && third_ent->flags & BLOCK) {
      second_ent->flags |= DELETED; // turn deleted bit on
      can_push = true;
    }
    else {
      can_push = false;
    }
  }

  if (can_push)
    move(first_ent, grid, second_x, second_y);

  return can_push;
}

int findAvailPos(entity* grid[]) {
  int x;
  int y;
  int pos;
  do {
    x = rand() % num_blocks_w;
    y = rand() % num_blocks_h;
    pos = to_pos(x, y);
  } while (grid[pos]);
  return pos;
}

void move(entity* ent, entity* grid[], int x, int y) {
  int prev_pos = to_pos(ent->x, ent->y);
  grid[prev_pos] = NULL;
  
  ent->x = x;
  ent->y = y;
  grid[to_pos(x, y)] = ent;
}

int to_x(int ix) {
  return ix % num_blocks_w;
}
int to_y(int ix) {
  return ix / num_blocks_w;
}
int to_pos(int x, int y) {
  int pos = x + y * num_blocks_w;
  if (pos < 0)
    error("position out of bounds (negative)");
  if (pos >= grid_len)
    error("position out of bounds (greater than grid size)");
  return pos;
}

void error(char* activity) {
  printf("%s failed: %s\n", activity, SDL_GetError());
  SDL_Quit();
  exit(-1);
}
