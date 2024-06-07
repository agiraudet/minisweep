#include "grid.h"
#include "raylib.h"
#include "win.h"
#include <stdint.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

int main(void) {
  SetTraceLogLevel(LOG_NONE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(60);

  struct grid *grid = grid_create(20, 20, 40);
  struct win win;
  win_init(&win, grid, SCREEN_WIDTH, SCREEN_HEIGHT);
  BeginDrawing();
  ClearBackground(BLACK);
  win_drawgrid(&win, grid);
  EndDrawing();

  while (!WindowShouldClose()) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      win_onlclic(&win, grid, GetMouseX(), GetMouseY());
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      win_onrclic(&win, grid, GetMouseX(), GetMouseY());
    }
    BeginDrawing();
    EndDrawing();
  }

  CloseWindow();
  grid_destroy(grid);
  return 0;
}
