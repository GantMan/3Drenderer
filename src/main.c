#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

bool is_running = false;

void setup(void) {
  // Initialize the window
  is_running = initialize_window();

  // Create a color buffer
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

  // Create a texture to map the color buffer to
  color_buffer_texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_ARGB8888, 
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, 
    window_height
  );

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

  draw_rect(150, 250, 200, 100, random_color());
 
  render_color_buffer();
  uint32_t some_color = 0xFFFFFF00; 
  clear_color_buffer(some_color);

  SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
  
  setup();

  // Game loop
  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();

  return 0;
} 