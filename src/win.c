#include "win.h"
#include "grid.h"
#include "raylib.h"
#include <stdint.h>

#define DEBUG_BOMB 0

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

void win_adujstoffset(struct win *win, struct grid *grid) {
  win->offx = win->offx / 2 + (win->width - grid->width * win->cell_size) / 2;
  win->offy = win->offy / 2 + (win->height - grid->height * win->cell_size) / 2;
}

void win_init(struct win *win, struct grid *grid, size_t width, size_t height,
              size_t x, size_t y) {
  if (!win || !grid)
    return;
  win->width = width;
  win->height = height;
  win->offx = x;
  win->offy = y;
  win->margin = 1;
  win->cell_size = min(width / grid->width, height / grid->height);
  win->draw_size = win->cell_size - (win->margin * 2);
  win_adujstoffset(win, grid);
}

Color win_getcellcolor(struct cell *cell) {
  if (cell->data == -1 && DEBUG_BOMB)
    return SKYBLUE;
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

  DrawRectangle(pos_x + win->offx, pos_y + win->offy, win->draw_size,
                win->draw_size, color);
  if (cell->status == REVEALED && cell->data > 0) {
    number_draw(cell->data, pos_x + win->draw_size / 2 - 2 + win->offx,
                pos_y + win->offy, win->draw_size);
  }
}

void win_drawgrid(struct win *win, struct grid *grid) {
  if (!win || !grid)
    return;
  for (size_t i = 0; i < grid->ncells; i++)
    win_drawcell(win, grid, i);
}

size_t win_posfromxy(struct win *win, struct grid *grid, int x, int y) {
  x -= win->offx;
  y -= win->offy;
  size_t cell_x = x / win->cell_size;
  size_t cell_y = y / win->cell_size;
  return cell_y * grid->width + cell_x;
}

int win_clicisinwin(struct win *win, struct grid *grid, int x, int y) {
  return (x > (int)win->offx &&
          x < (int)(win->cell_size * grid->width + win->offx) &&
          y > (int)win->offy &&
          y < (int)(win->cell_size * grid->height + win->offy));
}

void win_onrclic(struct win *win, struct grid *grid, int x, int y) {
  if (!win_clicisinwin(win, grid, x, y))
    return;
  size_t pos = win_posfromxy(win, grid, x, y);

  if (grid->cells[pos].status == REVEALED)
    return;
  grid->cells[pos].status ^= FLAGGED;
}

void win_onlclic(struct win *win, struct grid *grid, int x, int y) {
  static int first_clic = 1;

  if (!win_clicisinwin(win, grid, x, y))
    return;
  size_t pos = win_posfromxy(win, grid, x, y);
  if (first_clic && grid->cells[pos].data == -1)
    grid_cheatbomb(grid, pos);
  first_clic = 0;
  if (grid->cells[pos].status == REVEALED)
    grid_revealaroundcell(grid, pos);
  else
    grid_propagatecell(grid, pos, 0);
}
