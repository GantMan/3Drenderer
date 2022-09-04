#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool intialize_window(void) {
  // Make sure we can initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error");
    return false;
  } 

  // Create an SDL window 
  window = SDL_CreateWindow(
    NULL, 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    800, 
    600, 
    SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "SDL_CreateWindow Error");
    return false;
  } 

  // Create an SDL renderer 
  renderer = SDL_CreateRenderer(
    window, 
    -1, 
    0
  ); 

  if (!renderer) {
    fprintf(stderr, "SDL_CreateRenderer Error");
    return false;
  }
    
  return true;
}

void setup(void) {
  // Initialize the window
  is_running = intialize_window();
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT: {
      is_running = false;
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      break;
    }
  }
}

void update(void) {

}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 255, 21, 21, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  is_running = intialize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  return 0;
} 