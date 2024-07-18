#include "grid.h"
#include "raylib.h"
#include "win.h"
#include <stdint.h>
#include <stdio.h>

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

int main(void) {
  struct win win;
  struct grid *grid = grid_create(20, 20, 40);
  if (!grid)
    return 1;
  win_init(&win, grid, SCREEN_WIDTH - SCREEN_MARGIN,
           SCREEN_HEIGHT - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);

  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);
  BeginDrawing();
  ClearBackground(BLACK);
  win_drawgrid(&win, grid);
  EndDrawing();

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      win_init(&win, grid, GetScreenWidth() - SCREEN_MARGIN,
               GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);
    }
    if (grid->game_status == 0) {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        win_onlclic(&win, grid, GetMouseX(), GetMouseY());
        if (grid_checkwin(grid) != 0) {
          grid->time_end = GetTime();
          msg_end(grid);
        }
      }
      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        win_onrclic(&win, grid, GetMouseX(), GetMouseY());
      }
    }
    BeginDrawing();
    ClearBackground(BLACK);
    win_drawgrid(&win, grid);
    if (grid->game_status == 0)
      win_printtimer(&win, GetTime() - grid->time_start);
    else
      win_printtimer(&win, grid->time_end);
    EndDrawing();
  }

  CloseWindow();
  grid_destroy(grid);
  return 0;
}
