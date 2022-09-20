#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {0, 0, 0};

float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;

void setup(void) {
  // Intitalize enum values
  render_method = RENDER_SOLID_WIRE_VERTEX;
  cull_method = CULL_BACKFACE;

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

  load_cube_mesh_data();
  // load_obj_file_data("./assets/cube.obj");
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
      } else if (event.key.keysym.sym == SDLK_1) {
        render_method = RENDER_WIRE;
      } else if (event.key.keysym.sym == SDLK_2) {
        render_method = RENDER_WIRE_VERTEX;
      } else if (event.key.keysym.sym == SDLK_3) {
        render_method = RENDER_SOLID;
      } else if (event.key.keysym.sym == SDLK_4) {
        render_method = RENDER_SOLID_WIRE;
      } else if (event.key.keysym.sym == SDLK_5) {
        render_method = RENDER_SOLID_WIRE_VERTEX;
      } else if (event.key.keysym.sym == SDLK_c) {
        cull_method = CULL_BACKFACE;
      } else if (event.key.keysym.sym == SDLK_d) {
        cull_method = CULL_NONE;
      }
      break;
    }
  }
}

vec2_t project(vec3_t point) {
  vec2_t projected_point = {
    .x = (fov_factor * point.x / point.z),
    .y = (fov_factor * point.y / point.z)
  };
  return projected_point;
}

void update(void) {

  // Delay to match target framerate
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  // initialize array of triangles to render 
  triangles_to_render = NULL;

  mesh.rotation.x += 0.005;
  mesh.rotation.y += 0.001;
  mesh.rotation.z += 0.001;

  int num_faces = array_length(mesh.faces);
  for (int i = 0; i < num_faces; i++) {
    face_t mesh_face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[mesh_face.c - 1];

    vec3_t transformed_vertices[3];

    // Loop all 3 and apply transformations
    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];
      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // Translate the vertex away from camera
      transformed_vertex.z += 5;

      // Saved transformed vertex
      transformed_vertices[j] = transformed_vertex;
    }

    if (cull_method == CULL_BACKFACE) {
      // Perform backface culling 
      vec3_t vector_a = transformed_vertices[0];
      vec3_t vector_b = transformed_vertices[1];
      vec3_t vector_c = transformed_vertices[2];

      vec3_t vector_ab = vec3_sub(vector_b, vector_a);
      vec3_t vector_ac = vec3_sub(vector_c, vector_a);
      vec3_normalize(&vector_ab);
      vec3_normalize(&vector_ac);

      // Compute the face normal
      vec3_t normal = vec3_cross(vector_ab, vector_ac);
      vec3_normalize(&normal);

      // Find the vector between the triangle and the camera origin
      vec3_t camera_ray = vec3_sub(camera_position, vector_a);

      // calculate how aligned the normal is with the camera ray
      float dot_normal_camera = vec3_dot(normal, camera_ray);

      // If the dot product is negative, the normal is pointing away from the camera
      if (dot_normal_camera < 0) {
        continue;
      }

    }

    
    vec2_t projected_points[3];
    // loop all 3 vertices and project them
    for (int j = 0; j < 3; j++) {
      projected_points[j] = project(transformed_vertices[j]);

      // Scale and translate point to middle of the screen
      projected_points[j].x += window_width / 2;
      projected_points[j].y += window_height / 2;

      // projected_triangle.points[j] = projected_points;
    }
    triangle_t projected_triangle = {
      .points = {
        projected_points[0],
        projected_points[1],
        projected_points[2]
      },
      .color = mesh_face.color
    };

    array_push(triangles_to_render, projected_triangle);
  }


}

void render(void) {

  // Loop all projected points and render them
  int num_triangles = array_length(triangles_to_render);
  for (int i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];

    if (render_method == RENDER_WIRE_VERTEX || render_method == RENDER_SOLID_WIRE_VERTEX) {
      int vertex_radius = 3;
      draw_rect(triangle.points[0].x - vertex_radius, triangle.points[0].y - vertex_radius, vertex_radius*2, vertex_radius*2, 0xFFFF0000);
      draw_rect(triangle.points[1].x - vertex_radius, triangle.points[1].y - vertex_radius, vertex_radius*2, vertex_radius*2, 0xFFFF0000);
      draw_rect(triangle.points[2].x - vertex_radius, triangle.points[2].y - vertex_radius, vertex_radius*2, vertex_radius*2, 0xFFFF0000);
    }

    if (render_method == RENDER_SOLID || render_method == RENDER_SOLID_WIRE || render_method == RENDER_SOLID_WIRE_VERTEX)  {
      draw_filled_triangle(
        triangle.points[0].x, 
        triangle.points[0].y, 
        triangle.points[1].x, 
        triangle.points[1].y, 
        triangle.points[2].x, 
        triangle.points[2].y, 
        triangle.color
      );
    }

    if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_SOLID_WIRE || render_method == RENDER_SOLID_WIRE_VERTEX) {
      draw_triangle(
        triangle.points[0].x, 
        triangle.points[0].y, 
        triangle.points[1].x, 
        triangle.points[1].y, 
        triangle.points[2].x, 
        triangle.points[2].y, 
        0xFFFF00FF
      );    
    }
  }

  // clear array of triangles to render
  array_free(triangles_to_render);
 
  render_color_buffer();
  uint32_t some_color = 0xFF000000; 
  clear_color_buffer(some_color);

  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  free(color_buffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
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
  free_resources();

  return 0;
} 