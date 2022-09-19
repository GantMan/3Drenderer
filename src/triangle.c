#include "triangle.h"
#include "display.h"

void int_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, u_int32_t color) {
    float invslope1 = (x1 - x0) / (float)(y1 - y0);
    float invslope2 = (x2 - x0) / (float)(y2 - y0);

    float curx1 = x0;
    float curx2 = x0;

    for (int scanlineY = y0; scanlineY <= y1; scanlineY++) {
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

    for (int scanlineY = y2; scanlineY > y0; scanlineY--) {
        curx1 -= invslope1;
        curx2 -= invslope2;
        draw_line(curx1, scanlineY, curx2, scanlineY, color);
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