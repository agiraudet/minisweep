#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#define REVEALED 1
#define HIDDEN 0
#define FLAGGED -1

struct cell {
  int8_t data;
  int8_t status;
};

struct grid {
  size_t nbombs;
  size_t ncells;
  size_t width;
  size_t height;
  struct cell *cells;
};

void grid_foreacharound(struct grid *grid, size_t pos, void *data,
                        void f(struct grid *, size_t, void *));
void grid_propagatecell(struct grid *grid, size_t pos, void *data);
struct grid *grid_create(size_t width, size_t height, size_t nbombs);
void grid_destroy(struct grid *grid);
void grid_putbombs(struct grid *grid);
void grid_revealbombs(struct grid *grid);
void grid_revealaroundcell(struct grid *grid, size_t pos);
int grid_checkwin(struct grid *grid);
void grid_cheatbomb(struct grid *grid, size_t pos);

#endif
