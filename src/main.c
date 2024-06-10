#include "grid.h"
#include "raylib.h"
#include "win.h"
#include <stdint.h>
#include <stdio.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

int main(void) {
  struct win win;
  struct grid *grid = grid_create(20, 20, 40);
  if (!grid)
    return 1;
  win_init(&win, grid, SCREEN_WIDTH, SCREEN_HEIGHT);

  SetTraceLogLevel(LOG_NONE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);
  BeginDrawing();
  ClearBackground(BLACK);
  win_drawgrid(&win, grid);
  EndDrawing();

  int game_status = 0;

  while (!WindowShouldClose()) {
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
