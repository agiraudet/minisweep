#include "win.h"
#include "grid.h"
#include "raylib.h"
#include <stdint.h>

static size_t min(size_t a, size_t b) { return a <= b ? a : b; }

void number_draw(int8_t n, size_t x, size_t y, size_t size) {
  Color color;
  switch (n) {
  case 1:
    color = DARKBLUE;
    break;
  case 2:
    color = DARKGREEN;
    break;
  case 3:
    color = RED;
    break;
  case 4:
    color = PURPLE;
    break;
  case 5:
    color = ORANGE;
    break;
  default:
    color = WHITE;
  }
  DrawText(TextFormat("%i", n), x, y, size, color);
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

Color win_getcellcolor(struct cell *cell) {
  if (cell->status == HIDDEN)
    return DARKGRAY;
  if (cell->status == FLAGGED)
    return YELLOW;
  if (cell->data == -1)
    return RED;
  return GRAY;
}

void win_drawcell(struct win *win, struct grid *grid, size_t pos) {
  struct cell *cell = &grid->cells[pos];
  Color color = win_getcellcolor(cell);
  size_t pos_x = pos % grid->width * win->cell_size + win->margin;
  size_t pos_y = pos / grid->width * win->cell_size + win->margin;

  DrawRectangle(pos_x, pos_y, win->draw_size, win->draw_size, color);
  if (cell->status == REVEALED && cell->data > 0) {
    number_draw(cell->data, pos_x + win->draw_size / 2 - 2, pos_y,
                win->draw_size);
  }
}

void win_drawgrid(struct win *win, struct grid *grid) {
  if (!win || !grid)
    return;
  for (size_t i = 0; i < grid->ncells; i++)
    win_drawcell(win, grid, i);
}

size_t win_posfromxy(struct win *win, struct grid *grid, int x, int y) {
  size_t cell_x = x / win->cell_size;
  size_t cell_y = y / win->cell_size;
  return cell_y * grid->width + cell_x;
}

void win_onrclic(struct win *win, struct grid *grid, int x, int y) {
  size_t pos = win_posfromxy(win, grid, x, y);

  if (grid->cells[pos].status == REVEALED)
    return;
  grid->cells[pos].status ^= FLAGGED;
}

void win_onlclic(struct win *win, struct grid *grid, int x, int y) {
  size_t pos = win_posfromxy(win, grid, x, y);
  if (grid->cells[pos].status == REVEALED)
    grid_revealaroundcell(grid, pos);
  else
    grid_propagatecell(grid, pos, 0);
}
