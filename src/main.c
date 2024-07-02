#include "grid.h"
#include "raylib.h"
#include "win.h"
#include <stdint.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SCREEN_MARGIN 5

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

  int game_status = 0;

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      win_init(&win, grid, GetScreenWidth() - SCREEN_MARGIN,
               GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);
      ClearBackground(BLACK);
    }
    if (game_status == 0) {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        win_onlclic(&win, grid, GetMouseX(), GetMouseY());
        game_status = grid_checkwin(grid);
        if (game_status == 1)
          printf("Congrats !! You won\n");
        else if (game_status == -1)
          printf("Oh no...\n");
      }
      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        win_onrclic(&win, grid, GetMouseX(), GetMouseY());
      }
    }
    BeginDrawing();
    win_drawgrid(&win, grid);
    EndDrawing();
  }

  CloseWindow();
  grid_destroy(grid);
  return 0;
}
