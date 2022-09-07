#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
  // Make sure we can initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error");
    return false;
  } 

  // Use SDL to query the full-screen resolution
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

  // Create an SDL window 
  window = SDL_CreateWindow(
    NULL, 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    window_width, 
    window_height, 
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

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
  return true;
}

void draw_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
    color_buffer[(window_width * y) + x] = color;
  }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int current_x = x + i;
      int current_y = y + j;
      draw_pixel(current_x, current_y, color);
    }
  }
}

void draw_grid(int grid_size) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      uint32_t draw_color = (x % grid_size == 0 || y % grid_size == 0) ? random_color() : 0xFF000000;
      color_buffer[(window_width * y) + x] = draw_color;
    }
  }
}

void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(
    color_buffer_texture, 
    NULL, 
    color_buffer, 
    (int) (window_width * sizeof(uint32_t))
  );

  SDL_RenderCopy(
    renderer, 
    color_buffer_texture, 
    NULL, 
    NULL
  );

}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

uint32_t random_color() {
  uint8_t r = (uint8_t) (rand() % 255);
  uint8_t g = (uint8_t) (rand() % 255);
  uint8_t b = (uint8_t) (rand() % 255);
  return (r << 16) | (g << 8) | b;
}