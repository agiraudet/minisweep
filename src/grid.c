#include "grid.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void grid_foreacharound(void *data, struct grid *grid, size_t pos,
                        void f(void *, struct grid *, size_t)) {
  if (!grid)
    return;
  if (pos % grid->width) {
    // top-left
    if (pos - 1 >= grid->width)
      f(data, grid, pos - 1 - grid->width);
    // left
    if (pos > 0)
      f(data, grid, pos - 1);
    // bottom-left
    if (pos - 1 + grid->width < grid->ncells)
      f(data, grid, pos - 1 + grid->width);
  }
  if ((pos + 1) % grid->width) {
    // top-right
    if (pos + 1 >= grid->width)
      f(data, grid, pos + 1 - grid->width);
    // right
    if (pos + 1 < grid->ncells)
      f(data, grid, pos + 1);
    // bottom-right
    if (pos + 1 + grid->width < grid->ncells)
      f(data, grid, pos + 1 + grid->width);
  }
  // top
  if (pos >= grid->width)
    f(data, grid, pos - grid->width);
  // bottom
  if (pos + grid->width < grid->ncells)
    f(data, grid, pos + grid->width);
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
  grid->nrevelead = 0;
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

void grid_incdata(void *data, struct grid *grid, size_t pos) {
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
    grid_foreacharound(0, grid, pos, &grid_incdata);
    nbombs--;
  }
}
