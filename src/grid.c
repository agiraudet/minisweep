#include "grid.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void grid_foreacharound(struct grid *grid, size_t pos, void *data,
                        void f(struct grid *, size_t, void *)) {
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

void grid_propagatecell(struct grid *grid, size_t pos, void *data) {
  struct cell *cell = &grid->cells[pos];
  (void)data;

  if (cell->status == REVEALED)
    return;
  cell->status = REVEALED;
  if (cell->data == 0) {
    grid_foreacharound(grid, pos, 0, &grid_propagatecell);
  }
}

struct grid *grid_create(size_t width, size_t height, size_t nbombs) {
  struct grid *grid;

  if (nbombs > width * height)
    return 0;
  grid = malloc(sizeof(struct grid));
  if (!grid)
    return 0;
  grid->nbombs = nbombs;
  grid->width = width;
  grid->height = height;
  grid->ncells = width * height;
  grid->cells = calloc(width * height, sizeof(struct cell));
  if (!grid->cells) {
    free(grid);
    return 0;
  }
  grid_putbombs(grid);
  return grid;
}

void grid_destroy(struct grid *grid) {
  free(grid->cells);
  free(grid);
}

void grid_incdata(struct grid *grid, size_t pos, void *data) {
  (void)data;
  if (!grid)
    return;
  if (pos < grid->ncells && grid->cells[pos].data != -1)
    grid->cells[pos].data += 1;
}

void grid_putbombs(struct grid *grid) {
  size_t nbombs;

  if (!grid)
    return;
  nbombs = grid->nbombs;
  srand(time(NULL));
  while (nbombs) {
    size_t pos;
    do {
      pos = rand() % grid->ncells;
    } while (grid->cells[pos].data == -1);
    grid->cells[pos].data = -1;
    grid_foreacharound(grid, pos, 0, &grid_incdata);
    nbombs--;
  }
}

void grid_revealbombs(struct grid *grid) {
  if (!grid)
    return;
  for (size_t i = 0; i < grid->ncells; i++) {
    if (grid->cells[i].data == -1)
      grid->cells[i].status = REVEALED;
  }
}

void grid_countflaggedcell(struct grid *grid, size_t pos, void *data) {
  int8_t *count = data;

  if (grid->cells[pos].status == FLAGGED)
    (*count)++;
}

void grid_setrevaroundcell(struct grid *grid, size_t pos, void *data) {
  (void)data;
  if (grid->cells[pos].status != FLAGGED)
    grid_propagatecell(grid, pos, 0);
}

void grid_revealaroundcell(struct grid *grid, size_t pos) {
  if (!grid || grid->cells[pos].status != REVEALED)
    return;
  int8_t count = 0;
  grid_foreacharound(grid, pos, &count, &grid_countflaggedcell);
  if (count == grid->cells[pos].data)
    grid_foreacharound(grid, pos, 0, &grid_setrevaroundcell);
}

int grid_checkwin(struct grid *grid) {
  size_t nrevealed = 0;
  if (!grid)
    return 0;
  for (size_t i = 0; i < grid->ncells; i++) {
    if (grid->cells[i].data == -1 && grid->cells[i].status == REVEALED)
      return -1;
    if (grid->cells[i].status == REVEALED)
      nrevealed++;
  }
  if (nrevealed == grid->ncells - grid->nbombs)
    return 1;
  return 0;
}
