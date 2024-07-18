#include "win.h"
#include "grid.h"
#include "raylib.h"
#include "theme.h"
#include <stdint.h>
#include <stdio.h>

#define DEBUG_BOMB 0

static size_t min(size_t a, size_t b) { return a <= b ? a : b; }

void number_draw(int8_t n, size_t x, size_t y, size_t size) {
  n = n <= 5 ? n : 0;
  DrawText(TextFormat("%i", n), x, y, size, g_theme.numbers[n]);
}

void win_adujstoffset(struct win *win, struct grid *grid) {
  win->offx = win->offx / 2 + (win->width - grid->width * win->cell_size) / 2;
  win->offy = win->offy / 2 + (win->height - grid->height * win->cell_size) / 2;
  win->absw = GetScreenWidth();
  win->absh = GetScreenHeight();
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
    return g_theme.hidden;
  if (cell->status == FLAGGED)
    return g_theme.flagged;
  if (cell->data == -1)
    return g_theme.bomb;
  return g_theme.cell;
}

void win_drawcell(struct win *win, struct grid *grid, size_t pos) {
  struct cell *cell = &grid->cells[pos];
  Color color = win_getcellcolor(cell);
  size_t pos_x = pos % grid->width * win->cell_size + win->margin + win->offx;
  size_t pos_y = pos / grid->width * win->cell_size + win->margin + win->offy;

  DrawRectangle(pos_x, pos_y, win->draw_size, win->draw_size, color);
  if (cell->status == REVEALED && cell->data > 0) {
    char nbuf[] = {'0', 0};
    nbuf[0] += cell->data;
    size_t n_x =
        win->cell_size / 2 - MeasureText(nbuf, win->draw_size) / 2 + pos_x;
    size_t n_y = win->cell_size / 2 - win->draw_size / 2 + pos_y;
    number_draw(cell->data, n_x, n_y, win->draw_size);
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

void win_formattime(double timer, char *buffer, size_t buffer_size) {
  int hours = (int)timer / 3600;
  int minutes = ((int)timer % 3600) / 60;
  int seconds = (int)timer % 60;

  if (hours > 0) {
    snprintf(buffer, buffer_size, "%02d:%02d:%02d", hours, minutes, seconds);
  } else {
    snprintf(buffer, buffer_size, "%02d:%02d", minutes, seconds);
  }
}

void win_printtimer(struct win *win, double time) {
  char buf[25];

  win_formattime(time, buf, 24);
  size_t time_size = win->offy - 4;
  int time_x = win->absw / 2 - MeasureText(buf, time_size) / 2;
  int time_y = win->offy / 2 - time_size / 2;
  DrawText(TextFormat("%s", buf), time_x, time_y, time_size, g_theme.timer);
}
