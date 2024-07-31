#include "minisweep.h"
#include "menu.h"
#include "save.h"
#include "theme.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ms_setting_init(t_setting *sett) {
  sett->double_click = 0;
  sett->last_click = GetTime();
  sett->double_click_delay = 0.3f;
  sett->savefile_path = save_getpath();
}

void ms_destroy(t_minisweep *ms) {
  if (!ms)
    return;
  ms->menus[SCORE]->subtitle = 0;
  ms->save_data.theme_n = theme_switch(-1);
  save_writeraw(&ms->save_data, ms->sett.savefile_path);
  if (ms->sett.savefile_path)
    free(ms->sett.savefile_path);
  if (ms->grid)
    grid_destroy(ms->grid);
  if (ms->win)
    free(ms->win);
  for (int i = 0; ms->menus[i]; i++)
    menu_destroy(ms->menus[i]);
  if (ms->menus)
    free(ms->menus);
  free(ms);
}

int ms_create_menus(t_minisweep *ms) {
  if (!ms)
    return -1;
  ms->menus = calloc(N_MENU + 1, sizeof(t_menu *));
  size_t winw = GetScreenWidth();
  size_t winh = GetScreenHeight();
  ms->menus[MAIN] = menu_create_main(winw, winh);
  if (!ms->menus[MAIN]) {
    ms_destroy(ms);
    return -1;
  }
  ms->menus[END] = menu_create_end(400, 200);
  if (!ms->menus[END]) {
    ms_destroy(ms);
    return -1;
  }
  menu_mov(ms->menus[END], winw / 2 - ms->menus[END]->win_w / 2,
           winh / 2 - ms->menus[END]->win_h / 2);
  ms->menus[SETTING] = menu_create_setting(350, 500);
  menu_mov(ms->menus[SETTING], winw / 2 - ms->menus[SETTING]->win_w / 2,
           winh / 2 - ms->menus[SETTING]->win_h / 2);
  ms->menus[SCORE] = menu_create_score(winw * 0.3, 50);
  menu_mov(ms->menus[SCORE], winw / 2 - ms->menus[SCORE]->win_w / 2,
           winh - ms->menus[SCORE]->win_h * 1.5);
  return 0;
}

t_minisweep *ms_create(void) {
  t_minisweep *ms = malloc(sizeof(t_minisweep));
  if (!ms)
    return 0;
  ms_setting_init(&ms->sett);
  save_init(&ms->save_data);
  save_loadraw(&ms->save_data, ms->sett.savefile_path);
  // SETTINGS
  ms->sett.double_click = ms->save_data.double_clic;
  if (ms->save_data.theme_n >= 0)
    theme_switch(ms->save_data.theme_n);
  win_formattime(ms->save_data.hs_small, ms->sett.hs_small, 25);
  win_formattime(ms->save_data.hs_medium, ms->sett.hs_medium, 25);
  win_formattime(ms->save_data.hs_large, ms->sett.hs_large, 25);
  //
  ms->alive = 1;
  ms->grid = 0;
  ms->win = malloc(sizeof(t_win));
  if (!ms->win) {
    ms_destroy(ms);
    return 0;
  }
  if (ms_create_menus(ms) != 0)
    return 0;
  return ms;
}

void ms_process_resize(t_minisweep *ms) {
  int sw = GetScreenWidth();
  int sh = GetScreenHeight();
  win_init(ms->win, ms->grid, sw - SCREEN_MARGIN, sh - SCREEN_MARGIN,
           SCREEN_MARGIN, SCREEN_MARGIN);
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    if (mn->draggable) {
      mn->x = sw / 2 - mn->win_w / 2;
      mn->y = sh / 2 - mn->win_h / 2;
      menu_update_size(mn, mn->win_w, mn->win_h);
    } else if (i != SCORE)
      menu_update_size(mn, sw, sh);
    else {
      menu_mov(ms->menus[SCORE], sw / 2 - ms->menus[SCORE]->win_w / 2,
               sh - ms->menus[SCORE]->win_h * 1.5);
    }
  }
}

void ms_process_mouseover(t_minisweep *ms) {
  int mouse_x = GetMouseX();
  int mouse_y = GetMouseY();
  for (int i = N_MENU - 1; i >= 0; i--) {
    t_menu *mn = ms->menus[i];
    if (mn && mn->visible) {
      const char *str = menu_find_clic(mn, mouse_x, mouse_y);
      if (str) {
        if (mn->onmouseover)
          mn->onmouseover(mn, ms, str);
        return;
      }
    }
  }
}

int ms_process_clicmenu(t_minisweep *ms, int clic_x, int clic_y) {
  int ret = 0;
  for (int i = N_MENU - 1; i >= 0; i--) {
    t_menu *mn = ms->menus[i];
    if (!mn)
      continue;
    if (mn->visible) {
      const char *str = menu_find_dragorclic(mn, clic_x, clic_y);
      if (str && mn->onclic) {
        mn->onclic(mn, ms, str);
        return 1;
      }
      ret = 1;
    }
  }
  return ret;
}

char *ms_get_hsbuf(t_minisweep *ms) {
  if (!ms || !ms->grid)
    return 0;
  if (ms->grid->hs == &ms->save_data.hs_small)
    return ms->sett.hs_small;
  else if (ms->grid->hs == &ms->save_data.hs_medium)
    return ms->sett.hs_medium;
  else if (ms->grid->hs == &ms->save_data.hs_large)
    return ms->sett.hs_large;
  return 0;
}

void process_endofgame(t_minisweep *ms) {
  int new_pb = 0;
  char buf[25];
  ms->grid->time_end = GetTime() - ms->grid->time_start;
  win_formattime(ms->grid->time_end, buf, 25);
  if (ms->grid->game_status == 1 && ms->grid->hs &&
      (ms->grid->time_end < *(ms->grid->hs) || *(ms->grid->hs) == 0)) {
    *(ms->grid->hs) = ms->grid->time_end;
    new_pb = 1;
  }
  if (ms->grid->game_status == 1) {
    if (new_pb) {
      menu_settitle(ms->menus[END], "  New PB!  ");
      char *hs_buf = ms_get_hsbuf(ms);
      strncpy(hs_buf, buf, 25);
    } else
      menu_settitle(ms->menus[END], "   Yay!   ");
  } else
    menu_settitle(ms->menus[END], "   Oh no   ");
  menu_setsubtitle(ms->menus[END], buf);
  ms->menus[END]->visible = 1;
}

void ms_process_clicgrid(t_minisweep *ms, int clic_x, int clic_y) {
  if (!ms || !ms->grid)
    return;
  if (ms->sett.double_click) {
    double current_time = GetTime();
    if (current_time - ms->sett.last_click > ms->sett.double_click_delay) {
      ms->sett.last_click = current_time;
      return;
    }
  }
  win_onlclic(ms->win, ms->grid, clic_x, clic_y);
  if (grid_checkwin(ms->grid) != 0)
    process_endofgame(ms);
}

int ms_menu_opened(t_minisweep *ms) {
  for (int i = N_MENU - 1; i >= 0; i--) {
    t_menu *mn = ms->menus[i];
    if (mn && mn->visible)
      return 1;
  }
  return 0;
}

void ms_process_input(t_minisweep *ms) {
  if (!ms)
    return;
  if (IsKeyReleased(KEY_F1))
    theme_next();
  if (IsKeyReleased(KEY_F2)) {
    ms->sett.double_click ^= 1;
    ms->save_data.double_clic = ms->sett.double_click;
    printf("double click: %d\n", ms->sett.double_click);
  }
  if (IsKeyReleased(KEY_ESCAPE)) {
    ms->menus[SETTING]->visible ^= 1;
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    int clic_x = GetMouseX();
    int clic_y = GetMouseY();
    if (!ms_process_clicmenu(ms, clic_x, clic_y) && ms->grid &&
        ms->grid->game_status == 0)
      ms_process_clicgrid(ms, clic_x, clic_y);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    for (int i = 0; ms->menus[i]; i++) {
      t_menu *mn = ms->menus[i];
      menu_undrag(mn);
    }
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && !ms_menu_opened(ms)) {
    int clic_x = GetMouseX();
    int clic_y = GetMouseY();
    if (ms->grid && ms->grid->game_status == 0)
      win_onrclic(ms->win, ms->grid, clic_x, clic_y);
  }
  for (int i = 0; ms->menus[i]; i++) {
    t_menu *mn = ms->menus[i];
    menu_movdrag(mn);
    if ((i == MAIN && !mn->visible) || (i > MAIN && i != SCORE && mn->visible))
      ms->menus[SCORE]->visible = 0;
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
