#include "grid.h"
#include "menu.h"
#include "raylib.h"
#include "setting.h"
#include "theme.h"
#include "win.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SCREEN_MARGIN 50
#define N_MENUS 3

t_setting g_setting;

void defaut_setting(void) {
  g_setting.double_click = 0;
  g_setting.last_click = GetTime();
  g_setting.double_click_delay = 0.3f;
}

int game_clic(void) {
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    if (g_setting.double_click == 0)
      return 1;
    else {
      int ret = 0;
      double current_time = GetTime();
      if (current_time - g_setting.last_click <= g_setting.double_click_delay)
        ret = 1;
      g_setting.last_click = current_time;
      return ret;
    }
  }
  return 0;
}

void game_loop(t_win *win, t_grid **gridptr, t_menu *endmn, int *menu_mode) {
  t_grid *grid = *gridptr;
  if (grid->game_status == 0) {
    if (game_clic()) {
      win_onlclic(win, grid, GetMouseX(), GetMouseY());
      if (grid_checkwin(grid) != 0) {
        grid->time_end = GetTime() - grid->time_start;
        char buf[25];
        win_formattime(grid->time_end, buf, 25);
        menu_setsubtitle(endmn, buf);
        menu_settitle(endmn,
                      grid->game_status == 1 ? "   Yay!   " : "   Oh no   ");
      }
    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
      win_onrclic(win, grid, GetMouseX(), GetMouseY());
    }
  } else {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      const char *str = menu_find_dragorclic(endmn, GetMouseX(), GetMouseY());
      if (str) {
        if (strcmp(str, "quit") == 0)
          *menu_mode = -1;
        else if (strcmp(str, "menu") == 0) {
          *menu_mode = 1;
          grid_destroy(grid);
          *gridptr = 0;
          return;
        }
      }
    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
      menu_undrag(endmn);
  }
  menu_movdrag(endmn);
  BeginDrawing();
  ClearBackground(g_theme.bg);
  win_drawgrid(win, grid);
  if (grid->game_status == 0)
    win_printtimer(win, GetTime() - grid->time_start);
  else {
    win_printtimer(win, grid->time_end);
    menu_draw(endmn);
  }
  EndDrawing();
}

void menu_loop(t_menu *mn, t_win *win, t_grid **grid, int *menu_mode) {

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    const char *str = menu_find_clic(mn, GetMouseX(), GetMouseY());
    if (str) {
      if (strcmp(str, "small") == 0) {
        *grid = grid_create(10, 10, 10);
        *menu_mode = 0;
      } else if (strcmp(str, "medium") == 0) {
        *grid = grid_create(20, 20, 45);
        *menu_mode = 0;
      } else if (strcmp(str, "large") == 0) {
        *grid = grid_create(30, 30, 150);
        *menu_mode = 0;
      } else if (strcmp(str, "change theme") == 0) {
        next_theme();
      }
      if (*menu_mode == 0) {
        win_init(win, *grid, GetScreenWidth() - SCREEN_MARGIN,
                 GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN,
                 SCREEN_MARGIN);
      }
    }
  }
  BeginDrawing();
  ClearBackground(g_theme.bg);
  menu_draw(mn);
  EndDrawing();
}

int main2(void) {
  next_theme();
  defaut_setting();
  t_win win;
  t_grid *grid = 0;

  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);

  t_menu *mn = menu_create_main(SCREEN_WIDTH, SCREEN_HEIGHT);
  t_menu *endmn = menu_create_end(400, 200);
  menu_mov(endmn, GetScreenWidth() / 2 - endmn->win_w / 2,
           GetScreenHeight() / 2 - endmn->win_h / 2);

  int menu_mode = 1;
  while (!WindowShouldClose() && menu_mode != -1) {
    if (IsKeyReleased(KEY_F1))
      next_theme();
    if (IsKeyReleased(KEY_F2)) {
      g_setting.double_click ^= 1;
      printf("double click: %d\n", g_setting.double_click);
    }
    if (IsWindowResized()) {
      int sw = GetScreenWidth();
      int sh = GetScreenHeight();
      win_init(&win, grid, sw - SCREEN_MARGIN, sh - SCREEN_MARGIN,
               SCREEN_MARGIN, SCREEN_MARGIN);
      endmn->x = sw / 2 - endmn->win_w / 2;
      endmn->y = sh / 2 - endmn->win_h / 2;
      menu_update_size(endmn, endmn->win_w, endmn->win_h);
      menu_update_size(mn, sw, sh);
    }
    if (menu_mode) {
      menu_loop(mn, &win, &grid, &menu_mode);
    } else {
      game_loop(&win, &grid, endmn, &menu_mode);
    }
  }

  CloseWindow();
  if (grid)
    grid_destroy(grid);
  menu_destroy(mn);
  menu_destroy(endmn);
  return 0;
}

////////////////////////////////

typedef struct s_minisweep {
  t_menu **menus;
  t_win *win;
  t_grid *grid;
  t_setting sett;
  int alive;
} t_minisweep;

void ms_setting_init(t_setting *sett) {
  sett->double_click = 0;
  sett->last_click = GetTime();
  sett->double_click_delay = 0.3f;
}

void ms_destroy(t_minisweep *ms) {
  if (!ms)
    return;
  if (ms->grid)
    grid_destroy(ms->grid);
  if (ms->win)
    free(ms->win);
  t_menu *mn = *ms->menus;
  while (mn)
    menu_destroy(mn++);
  if (ms->menus)
    free(ms->menus);
  free(ms);
}

t_minisweep *ms_create(void) {
  t_minisweep *ms = malloc(sizeof(t_minisweep));
  if (!ms)
    return 0;
  ms->alive = 1;
  ms->grid = 0;
  ms->win = malloc(sizeof(t_win));
  if (!ms->win) {
    ms_destroy(ms);
    return 0;
  }
  ms->menus = calloc(N_MENUS + 1, sizeof(t_menu *));
  size_t winw = GetScreenWidth();
  size_t winh = GetScreenHeight();
  ms->menus[0] = menu_create_main(winw, winh);
  if (!ms->menus[0]) {
    ms_destroy(ms);
    return 0;
  }
  ms->menus[1] = menu_create_end(400, 200);
  if (!ms->menus[1]) {
    ms_destroy(ms);
    return 0;
  }
  menu_mov(ms->menus[1], winw / 2 - ms->menus[1]->win_w / 2,
           winh / 2 - ms->menus[1]->win_h / 2);
  ms->menus[2] = 0;
  ms_setting_init(&ms->sett);
  return ms;
}

int ms_process_clicmenu(t_minisweep *ms, int clic_x, int clic_y) {
  for (t_menu *mn = *ms->menus; mn; mn++) {
    if (mn->visible) {
      const char *str = menu_find_dragorclic(mn, clic_x, clic_y);
      if (str) {
        mn->onclic(mn, ms, str);
        return 1;
      }
    }
  }
  return 0;
}

void ms_process_clicgrid(t_minisweep *ms, int clic_x, int clic_y) {
  if (!ms || !ms->grid)
    return;
  if (ms->grid->game_status == 0) {
    win_onlclic(ms->win, ms->grid, clic_x, clic_y);
    if (grid_checkwin(ms->grid) != 0) {
    }
  }
}

void ms_process_input(t_minisweep *ms) {
  if (!ms)
    return;
  if (IsKeyReleased(KEY_F1))
    next_theme();
  if (IsKeyReleased(KEY_F2)) {
    ms->sett.double_click ^= 1;
    printf("double click: %d\n", ms->sett.double_click);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    int clic_x = GetMouseX();
    int clic_y = GetMouseY();
    if (!ms_process_clicmenu(ms, clic_x, clic_y)) {
    }
  }
}

void ms_draw(t_minisweep *ms) {
  if (!ms)
    return;
  BeginDrawing();
  ClearBackground(g_theme.bg);
  if (ms->grid)
    win_drawgrid(ms->win, ms->grid);
  for (t_menu *mn = *ms->menus; mn; mn++) {
    if (mn->visible)
      menu_draw(mn);
  }
  EndDrawing();
}

void menu_main_onclic(t_menu *mn, t_minisweep *ms, const char *str) {

  if (!str)
    return;
  if (strcmp(str, "small") == 0) {
    ms->grid = grid_create(10, 10, 10);
    mn->visible = 0;
  } else if (strcmp(str, "medium") == 0) {
    ms->grid = grid_create(20, 20, 45);
    mn->visible = 0;
  } else if (strcmp(str, "large") == 0) {
    ms->grid = grid_create(30, 30, 150);
    mn->visible = 0;
  } else if (strcmp(str, "change theme") == 0) {
    next_theme();
  }
  if (!mn->visible)
    win_init(ms->win, ms->grid, GetScreenWidth() - SCREEN_MARGIN,
             GetScreenHeight() - SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_MARGIN);
}

void menu_end_onclic(t_menu *mn, t_minisweep *ms, const char *str) {
  if (!str)
    return;
  if (strcmp(str, "quit") == 0)
    ms->alive = 0;
  else if (strcmp(str, "menu") == 0) {
    grid_destroy(ms->grid);
    ms->grid = 0;
    return;
  }
}

void init_raylib(void) {
  SetTraceLogLevel(LOG_NONE);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "minisweep");
  SetTargetFPS(30);
}

int main(void) {
  init_raylib();
  t_minisweep *ms = ms_create();
  if (!ms) {
    CloseWindow();
    return 1;
  }
  while (!WindowShouldClose() && ms->alive) {
    // pass
  }

  CloseWindow();
  ms_destroy(ms);
  return 0;
}
