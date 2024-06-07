#include "win.h"
#include "grid.h"
#include "raylib.h"
#include <stdint.h>

static size_t min(size_t a, size_t b) { return a <= b ? a : b; }

void number_draw(int8_t n, size_t x, size_t y, size_t size) {
  if (n > 9 || n < 0)
    n = 8;
  DrawText(TextFormat("%i", n), x, y, size, WHITE);
}

void win_init(struct win *win, struct grid *grid, size_t width, size_t height) {
  if (!win || !grid)
    return;
  win->width = width;
  win->height = height;
  win->margin = 1;
  win->cell_size = min(width / grid->width, height / grid->height);
  win->draw_size = win->cell_size - (win->margin * 2);
}

void win_drawcell(struct win *win, struct grid *grid, size_t pos,
                  int8_t reveal) {
  size_t pos_x = pos % grid->width * win->cell_size + win->margin;
  size_t pos_y = pos / grid->width * win->cell_size + win->margin;

  if (reveal == 0) {
    DrawRectangle(pos_x, pos_y, win->draw_size, win->draw_size, DARKGRAY);
  } else if (reveal == -1) {
    DrawRectangle(pos_x, pos_y, win->draw_size, win->draw_size, YELLOW);
  } else {
    int8_t cell_data = grid->cells[pos].data;
    DrawRectangle(pos_x, pos_y, win->draw_size, win->draw_size,
                  cell_data == -1 ? RED : GRAY);
    if (cell_data > 0) {
      number_draw(cell_data, pos_x + win->draw_size / 2 - 2, pos_y,
                  win->draw_size);
    }
  }
}

void win_drawgrid(struct win *win, struct grid *grid) {
  if (!win || !grid)
    return;
  for (size_t i = 0; i < grid->ncells; i++)
    win_drawcell(win, grid, i, 0);
}

void win_showbombs(struct win *win, struct grid *grid) {
  for (size_t i = 0; i < grid->ncells; i++) {
    if (grid->cells[i].data == -1)
      win_drawcell(win, grid, i, 1);
  }
}

void win_propagatecell(void *windata, struct grid *grid, size_t pos) {
  struct win *win = windata;
  struct cell *cell = &grid->cells[pos];
  if (cell->revelead)
    return;
  if (cell->data >= 0) {
    win_drawcell(win, grid, pos, 1);
    cell->revelead = 1;
  }
  if (cell->data == 0) {
    grid_foreacharound(win, grid, pos, &win_propagatecell);
  }
}

size_t win_posfromxy(struct win *win, struct grid *grid, int x, int y) {
  size_t cell_x = x / win->cell_size;
  size_t cell_y = y / win->cell_size;
  return cell_y * grid->width + cell_x;
}

void win_onrclic(struct win *win, struct grid *grid, int x, int y) {
  size_t pos = win_posfromxy(win, grid, x, y);
  win_drawcell(win, grid, pos, -1);
}

void win_onlclic(struct win *win, struct grid *grid, int x, int y) {
  size_t pos = win_posfromxy(win, grid, x, y);
  if (grid->cells[pos].data == -1) {
    win_showbombs(win, grid);
    return;
  }
  win_propagatecell(win, grid, pos);
}
