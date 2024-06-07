#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

struct cell {
  int8_t data;
  uint8_t revelead;
};

struct grid {
  size_t nbombs;
  size_t ncells;
  size_t width;
  size_t height;
  size_t nrevelead;
  struct cell *cells;
};

void grid_foreacharound(void *data, struct grid *grid, size_t pos,
                        void f(void *, struct grid *, size_t));
struct grid *grid_create(size_t width, size_t height, size_t nbombs);
void grid_destroy(struct grid *grid);
void grid_putbombs(struct grid *grid);

#endif
