#include "minisweep.h"
#include "save.h"
#include "theme.h"
#include <raylib.h>
#include <string.h>

void init_raylib(void) {
  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);
}

int main(void) {
  init_raylib();
  next_theme();
  t_minisweep *ms = ms_create();
  if (!ms) {
    CloseWindow();
    return 1;
  }
  while (!WindowShouldClose() && ms->alive) {
    if (IsWindowResized())
      ms_process_resize(ms);
    ms_process_input(ms);
    ms_draw(ms);
  }
  CloseWindow();
  ms_destroy(ms);
  return 0;
}
