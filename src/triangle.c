#include "triangle.h"
#include "display.h"
#include "swap.h"

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u_int32_t color) {
    float invslope1 = (x1 - x0) / (float)(y1 - y0);
    float invslope2 = (x2 - x0) / (float)(y2 - y0);

    float curx1 = x0;
    float curx2 = x0;

    for (int scanlineY = y0; scanlineY <= y2; scanlineY++) {
        draw_line(curx1, scanlineY, curx2, scanlineY, color);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u_int32_t color) {
    float invslope1 = (x2 - x0) / (float)(y2 - y0);
    float invslope2 = (x2 - x1) / (float)(y2 - y1);

    float curx1 = x2;
    float curx2 = x2;

    for (int scanlineY = y2; scanlineY >= y0; scanlineY--) {
        draw_line(curx1, scanlineY, curx2, scanlineY, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

// Done via flat bottom and flat top triangles
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u_int32_t color) {
  // Sort the vertices by y coordinate ascending (y0 <= y1 <= y2)
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
  }

  if (y1 == y2) {
    fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
  } else if (y0 == y1) {
    fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
  } else {
    // calculate the new vertices MX and MY
    int My = y1;
    int Mx = ((float)(x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0;

    fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
    fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
  }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
  vec2_t ab = vec2_sub(b, a);
  vec2_t bc = vec2_sub(c, b);
  vec2_t ac = vec2_sub(c, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t bp = vec2_sub(p, b);

  // area of full triangle
  float area_triangle_abc = vec2_cross(ab, ac);

  // area of sub-triangles
  float alpha = vec2_cross(bc, bp) / area_triangle_abc;
  float beta = vec2_cross(ap, ac) / area_triangle_abc;
  float gamma = 1.0f - alpha - beta;

  vec3_t weights = {alpha, beta, gamma};
  return weights;
}

void draw_texel(
  int x, int y, uint32_t* texture, 
  vec4_t point_a, vec4_t point_b, vec4_t point_c,
  float u0, float v0, float u1, float v1, float u2, float v2
) {
  vec2_t a = vec2_from_vec4(point_a);
  vec2_t b = vec2_from_vec4(point_b);
  vec2_t c = vec2_from_vec4(point_c);

  vec3_t weights = barycentric_weights(a, b, c, (vec2_t){x, y});

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // Variables for interpolated values with 1/w for current pixel depth
  float interpolated_u;
  float interpolated_v;
  float interpolated_reciprocal_w;

  // Interpolate u, v, and 1/w (perspective correct on textures)
  interpolated_u = alpha * (u0 / point_a.w) + beta * (u1 / point_b.w) + gamma * (u2 / point_c.w);
  interpolated_v = alpha * (v0 / point_a.w) + beta * (v1  / point_b.w) + gamma * (v2 / point_c.w);

  // Also interpolate the value of 1/w for the current pixel
  interpolated_reciprocal_w = alpha * (1.0f / point_a.w) + beta * (1.0f / point_b.w) + gamma * (1.0f / point_c.w);
  // Now we divide back both interpolated values by 1/w to get the correct values
  interpolated_u /= interpolated_reciprocal_w;
  interpolated_v /= interpolated_reciprocal_w;

  int tex_x = abs((int)(interpolated_u * texture_width));
  int tex_y = abs((int)(interpolated_v * texture_height));

  uint32_t texel = texture[tex_y * texture_width + tex_x];
  draw_pixel(x, y, texel);
}

void draw_textured_triangle (
  int x0, int y0, float z0, float w0, float u0, float v0, 
  int x1, int y1, float z1, float w1, float u1, float v1, 
  int x2, int y2, float z2, float w2, float u2, float v2,
  uint32_t* texture
) {
  // Sort the vertices by y coordinate ascending (y0 <= y1 <= y2)
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&u1, &u2);
    float_swap(&v1, &v2);
    float_swap(&z1, &z2);
    float_swap(&w1, &w2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }

  // Create vector points
  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};

  // Render upper part (flatbottom)
  // Calculate the inverse slopes
  float invslope1 = 0;
  float invslope2 = 0;

  // Div by zero guard
  if (y1 - y0 != 0) {
    invslope1 = (x1 - x0) / (float) abs(y1 - y0);
  }
  if (y2 - y0 != 0) {
    invslope2 = (x2 - x0) / (float) abs(y2 - y0);
  }

  if (y1 - y0 != 0){
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * invslope1;
      int x_end = x0 + (y - y0) * invslope2;

      // assure x_start is to the right of x_end
      if (x_end < x_start) {
        int_swap(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
      }
    }
  }

  // Render lower part (flattop)
  invslope1 = 0;
  invslope2 = 0;

  if (y2 - y1 != 0) {
    invslope1 = (x2 - x1) / (float) abs(y2 - y1);
  }
  if (y2 - y0 != 0) {
    invslope2 = (x2 - x0) / (float) abs(y2 - y0);
  }

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * invslope1;
      int x_end = x0 + (y - y0) * invslope2;

      // assure x_start is to the right of x_end
      if (x_end < x_start) {
        int_swap(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_texel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
      }
    }
  }
}