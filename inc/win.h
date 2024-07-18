#ifndef WIN_H
#define WIN_H

#include "grid.h"

#include <stddef.h>
#include <stdint.h>

struct win {
  size_t width;
  size_t height;
  size_t offx;
  size_t offy;
  size_t absw;
  size_t absh;
  size_t cell_size;
  size_t margin;
  size_t draw_size;
};

void number_draw(int8_t n, size_t x, size_t y, size_t size);
void win_init(struct win *win, struct grid *grid, size_t width, size_t height,
              size_t x, size_t y);
void win_drawcell(struct win *win, struct grid *grid, size_t pos);
void win_drawgrid(struct win *win, struct grid *grid);
void win_onlclic(struct win *win, struct grid *grid, int x, int y);
void win_onrclic(struct win *win, struct grid *grid, int x, int y);
void win_printtimer(struct win *win, double time);
void win_formattime(double timer, char *buffer, size_t buffer_size);

#endif
