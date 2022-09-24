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

void draw_textured_triangle (
  int x0, int y0, float u0, float v0, 
  int x1, int y1, float u1, float v1, 
  int x2, int y2, float u2, float v2,
  uint32_t* texture
) {
  // Sort the vertices by y coordinate ascending (y0 <= y1 <= y2)
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&u1, &u2);
    float_swap(&v1, &v2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }

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
        draw_pixel(x, y, 0xFFFF00FF);
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
        draw_pixel(x, y, glitter());
      }
    }
  }
}