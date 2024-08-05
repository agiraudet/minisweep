#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

typedef enum { FLAGGED = -1, HIDDEN = 0, REVEALED = 1 } e_cellstate;

typedef struct cell {
  int8_t data;
  e_cellstate status;
} t_cell;

typedef struct grid {
  double time_start;
  double time_end;
  double *hs;
  int game_status;
  int bomb_cheat;
  size_t nbombs;
  size_t ncells;
  size_t width;
  size_t height;
  struct cell *cells;
} t_grid;

void grid_foreacharound(t_grid *grid, size_t pos, void *data,
                        void f(t_grid *, size_t, void *));
void grid_propagatecell(t_grid *grid, size_t pos, void *data);
t_grid *grid_create(size_t width, size_t height, size_t nbombs);
void grid_destroy(t_grid *grid);
void grid_putbombs(t_grid *grid);
void grid_revealbombs(t_grid *grid);
void grid_revealaroundcell(t_grid *grid, size_t pos);
int grid_checkwin(t_grid *grid);
void grid_cheatbomb(t_grid *grid, size_t pos);

#endif
