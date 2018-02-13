#include "include/SDL.h"
#include <stdbool.h>

int main(int num_args, char** args) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  window = SDL_CreateWindow("Beast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);

  SDL_Renderer* renderer = NULL;
  renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Rect r;
  r.x = 50;
  r.y = 50;
  r.w = 15;
  r.h = 20;

  SDL_Event evt;
  bool is_gameover = false;

  while (!is_gameover) {
    if (SDL_PollEvent(&evt)) {
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
            r.x -= 15;
            break;
          case SDLK_RIGHT:
            r.x += 15;
            break;
          case SDLK_UP:
            r.y -= 20;
            break;
          case SDLK_DOWN:
            r.y += 20;
            break;
        }
        break;
      }
    }

    // set BG color
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderClear(renderer);

    // set block color
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &r);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
//    SDL_UpdateRect(screen, 0, 0, 0, 0);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
