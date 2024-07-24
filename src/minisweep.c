#include "minisweep.h"
#include "menu.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

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
  // t_menu *mn = *ms->menus;
  // while (mn)
  for (int i = 0; ms->menus[i]; i++)
    menu_destroy(ms->menus[i]);
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
  ms->menus = calloc(N_MENU + 1, sizeof(t_menu *));
  size_t winw = GetScreenWidth();
  size_t winh = GetScreenHeight();
  ms->menus[MAIN] = menu_create_main(winw, winh);
  if (!ms->menus[MAIN]) {
    ms_destroy(ms);
    return 0;
  }
  ms->menus[END] = menu_create_end(400, 200);
  if (!ms->menus[END]) {
    ms_destroy(ms);
    return 0;
  }
  menu_mov(ms->menus[END], winw / 2 - ms->menus[END]->win_w / 2,
           winh / 2 - ms->menus[END]->win_h / 2);
  ms->menus[SETTING] = 0;
  ms_setting_init(&ms->sett);
  return ms;
}

void ms_process_resize(t_minisweep *ms) {
  int sw = GetScreenWidth();
  int sh = GetScreenHeight();
  win_init(ms->win, ms->grid, sw - SCREEN_MARGIN, sh - SCREEN_MARGIN,
           SCREEN_MARGIN, SCREEN_MARGIN);
  // for (t_menu *mn = ms->menus[0]; mn; mn++) {
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    if (mn->draggable) {
      mn->x = sw / 2 - mn->win_w / 2;
      mn->y = sh / 2 - mn->win_h / 2;
      menu_update_size(mn, mn->win_w, mn->win_h);
    } else
      menu_update_size(mn, sw, sh);
  }
}

int ms_process_clicmenu(t_minisweep *ms, int clic_x, int clic_y) {
  // for (t_menu *mn = ms->menus[0]; mn; mn++) {
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    if (mn->visible) {
      const char *str = menu_find_dragorclic(mn, clic_x, clic_y);
      if (str && mn->onclic) {
        mn->onclic(mn, ms, str);
        return 1;
      }
      menu_undrag(mn);
    }
  }
  return 0;
}

void ms_process_clicgrid(t_minisweep *ms, int clic_x, int clic_y) {
  if (!ms || !ms->grid)
    return;
  if (ms->grid->game_status != 0) {
    printf("WTF\n");
    return;
  }
  win_onlclic(ms->win, ms->grid, clic_x, clic_y);
  if (grid_checkwin(ms->grid) != 0) {
    char buf[25];
    ms->grid->time_end = GetTime() - ms->grid->time_start;
    win_formattime(ms->grid->time_end, buf, 25);
    menu_settitle(ms->menus[END],
                  ms->grid->game_status == 1 ? "   Yay!   " : "   Oh no   ");
    menu_setsubtitle(ms->menus[END], buf);
    ms->menus[END]->visible = 1;
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
    if (!ms_process_clicmenu(ms, clic_x, clic_y))
      ms_process_clicgrid(ms, clic_x, clic_y);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
    int clic_x = GetMouseX();
    int clic_y = GetMouseY();
    if (ms->grid && ms->grid->game_status == 0)
      win_onrclic(ms->win, ms->grid, clic_x, clic_y);
  }
  // for (t_menu *mn = ms->menus[0]; mn; mn++)
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    menu_movdrag(mn);
  }
}

void ms_draw(t_minisweep *ms) {
  if (!ms)
    return;
  BeginDrawing();
  ClearBackground(g_theme.bg);
  if (ms->grid)
    win_drawgrid(ms->win, ms->grid);
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    if (mn->visible)
      menu_draw(mn);
  }
  EndDrawing();
}
