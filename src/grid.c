#include "grid.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

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

void grid_incdata(struct grid *grid, size_t pos) {
  if (!grid)
    return;
  if (pos < grid->ncells && grid->cells[pos].data != -1)
    grid->cells[pos].data += 1;
}

void grid_around_incdata(struct grid *grid, size_t pos) {
  if (!grid)
    return;
  if (pos % grid->width) {
    grid_incdata(grid, pos - grid->width - 1);
    grid_incdata(grid, pos + grid->width - 1);
    grid_incdata(grid, pos - 1);
  }
  if (pos > grid->width)
    grid_incdata(grid, pos - grid->width);
  if (pos < grid->ncells - grid->width)
    grid_incdata(grid, pos + grid->width);
  if ((pos + 1) % grid->width) {
    grid_incdata(grid, pos - grid->width + 1);
    grid_incdata(grid, pos + 1);
    grid_incdata(grid, pos + grid->width + 1);
  }
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
    grid_around_incdata(grid, pos);
    nbombs--;
  }
}
