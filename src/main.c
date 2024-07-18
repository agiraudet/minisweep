#include "grid.h"
#include "menu.h"
#include "raylib.h"
#include "win.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SCREEN_MARGIN 50

void msg_end(struct grid *grid) {
  if (grid->game_status == 1) {
    char buf[25];
    win_formattime(grid->time_end - grid->time_start, buf, 24);
    printf(
        "\033[1;31mCongratulation !!!\033[0m\nWon %lux%lu (%lu bombs) in %s\n",
        grid->width, grid->height, grid->nbombs, buf);
  } else {
    printf("Oh no...\n");
  }
}

void game_loop(struct win *win, struct grid *grid) {
  if (IsWindowResized()) {
    win_init(win, grid, GetScreenWidth() - SCREEN_MARGIN,
             GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);
  }
  if (grid->game_status == 0) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      win_onlclic(win, grid, GetMouseX(), GetMouseY());
      if (grid_checkwin(grid) != 0) {
        grid->time_end = GetTime();
        msg_end(grid);
      }
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      win_onrclic(win, grid, GetMouseX(), GetMouseY());
    }
  }
  BeginDrawing();
  ClearBackground(BLACK);
  win_drawgrid(win, grid);
  if (grid->game_status == 0)
    win_printtimer(win, GetTime() - grid->time_start);
  else
    win_printtimer(win, grid->time_end);
  EndDrawing();
}

void menu_loop(menu *mn, struct win *win, struct grid **grid, int *menu_mode) {

  if (IsWindowResized()) {
    menu_update_size(mn, GetScreenWidth(), GetScreenHeight());
  }
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    const char *str = menu_find_clic(mn, GetMouseX(), GetMouseY());
    if (strcmp(str, "small") == 0) {
      *grid = grid_create(10, 10, 10);
      *menu_mode = 0;
    } else if (strcmp(str, "medium") == 0) {
      *grid = grid_create(20, 20, 40);
      *menu_mode = 0;
    } else if (strcmp(str, "large") == 0) {
      *grid = grid_create(30, 30, 90);
      *menu_mode = 0;
    }
    if (*menu_mode == 0) {
      win_init(win, *grid, GetScreenWidth() - SCREEN_MARGIN,
               GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);
    }
  }
  BeginDrawing();
  ClearBackground(BLACK);
  menu_draw(mn);
  EndDrawing();
}

int main(void) {
  struct win win;
  struct grid *grid = 0;

  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);

  menu *mn = menu_create_main(SCREEN_WIDTH, SCREEN_HEIGHT);

  int menu_mode = 1;
  while (!WindowShouldClose()) {
    if (menu_mode) {
      menu_loop(mn, &win, &grid, &menu_mode);
    } else {
      game_loop(&win, grid);
    }
  }

  CloseWindow();
  if (grid)
    grid_destroy(grid);
  return 0;
}
