#include "grid.h"
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void grid_foreacharound(t_grid *grid, size_t pos, void *data,
                        void f(t_grid *, size_t, void *)) {
  if (!grid)
    return;
  if (pos % grid->width) {
    // top-left
    if (pos - 1 >= grid->width)
      f(grid, pos - 1 - grid->width, data);
    // left
    if (pos > 0)
      f(grid, pos - 1, data);
    // bottom-left
    if (pos - 1 + grid->width < grid->ncells)
      f(grid, pos - 1 + grid->width, data);
  }
  if ((pos + 1) % grid->width) {
    // top-right
    if (pos + 1 >= grid->width)
      f(grid, pos + 1 - grid->width, data);
    // right
    if (pos + 1 < grid->ncells)
      f(grid, pos + 1, data);
    // bottom-right
    if (pos + 1 + grid->width < grid->ncells)
      f(grid, pos + 1 + grid->width, data);
  }
  // top
  if (pos >= grid->width)
    f(grid, pos - grid->width, data);
  // bottom
  if (pos + grid->width < grid->ncells)
    f(grid, pos + grid->width, data);
}

void grid_propagatecell(t_grid *grid, size_t pos, void *data) {
  t_cell *cell = &grid->cells[pos];
  (void)data;

  if (cell->status == REVEALED)
    return;
  cell->status = REVEALED;
  if (cell->data == 0) {
    grid_foreacharound(grid, pos, 0, &grid_propagatecell);
  }
}

t_grid *grid_create(size_t width, size_t height, size_t nbombs) {
  t_grid *grid;

  if (nbombs > width * height)
    return 0;
  grid = malloc(sizeof(t_grid));
  if (!grid)
    return 0;
  grid->time_start = GetTime();
  grid->time_end = 0;
  grid->hs = 0;
  grid->game_status = 0;
  grid->bomb_cheat = 1;
  grid->nbombs = nbombs;
  grid->width = width;
  grid->height = height;
  grid->ncells = width * height;
  grid->cells = calloc(width * height, sizeof(t_cell));
  if (!grid->cells) {
    free(grid);
    return 0;
  }
  grid_putbombs(grid);
  return grid;
}

void grid_destroy(t_grid *grid) {
  free(grid->cells);
  free(grid);
}

void grid_incdata(t_grid *grid, size_t pos, void *data) {
  (void)data;
  if (!grid)
    return;
  if (pos < grid->ncells && grid->cells[pos].data != -1)
    grid->cells[pos].data += 1;
}

void grid_decdata(t_grid *grid, size_t pos, void *data) {
  (void)data;
  if (!grid)
    return;
  if (pos < grid->ncells && grid->cells[pos].data != -1)
    grid->cells[pos].data -= 1;
}

void grid_countbomb(t_grid *grid, size_t pos, void *data) {
  size_t *count = data;
  if (!grid)
    return;
  if (grid->cells[pos].data == -1)
    *count += 1;
}

void grid_putonebomb(t_grid *grid) {
  size_t pos;
  do {
    pos = rand() % grid->ncells;
  } while (grid->cells[pos].data == -1);
  grid->cells[pos].data = -1;
  grid_foreacharound(grid, pos, 0, &grid_incdata);
}

void grid_putbombs(t_grid *grid) {
  size_t nbombs;

  if (!grid)
    return;
  nbombs = grid->nbombs;
  srand(time(NULL));
  while (nbombs) {
    grid_putonebomb(grid);
    nbombs--;
  }
}

void grid_revealbombs(t_grid *grid) {
  if (!grid)
    return;
  for (size_t i = 0; i < grid->ncells; i++) {
    if (grid->cells[i].data == -1)
      grid->cells[i].status = REVEALED;
  }
}

void grid_countflaggedcell(t_grid *grid, size_t pos, void *data) {
  int8_t *count = data;

  if (grid->cells[pos].status == FLAGGED)
    (*count)++;
}

void grid_setrevaroundcell(t_grid *grid, size_t pos, void *data) {
  (void)data;
  if (grid->cells[pos].status != FLAGGED)
    grid_propagatecell(grid, pos, 0);
}

void grid_revealaroundcell(t_grid *grid, size_t pos) {
  if (!grid || grid->cells[pos].status != REVEALED)
    return;
  int8_t count = 0;
  grid_foreacharound(grid, pos, &count, &grid_countflaggedcell);
  if (count == grid->cells[pos].data)
    grid_foreacharound(grid, pos, 0, &grid_setrevaroundcell);
}

int grid_checkwin(t_grid *grid) {
  size_t nrevealed = 0;
  if (!grid)
    return 0;
  for (size_t i = 0; i < grid->ncells; i++) {
    if (grid->cells[i].data == -1 && grid->cells[i].status == REVEALED) {
      grid->game_status = -1;
      return grid->game_status;
    }
    if (grid->cells[i].status == REVEALED)
      nrevealed++;
  }
  if (nrevealed == grid->ncells - grid->nbombs) {
    grid->game_status = 1;
    return grid->game_status;
  }
  grid->game_status = 0;
  return grid->game_status;
}

void grid_cheatbomb(t_grid *grid, size_t pos) {
  if (!grid)
    return;
  grid_putonebomb(grid);
  size_t bombAround = 0;
  grid_foreacharound(grid, pos, &bombAround, &grid_countbomb);
  grid->cells[pos].data = bombAround;
  grid_foreacharound(grid, pos, 0, &grid_decdata);
}
