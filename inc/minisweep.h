#ifndef MINISWEEP_H
#define MINISWEEP_H

#include "menu.h"
#include "save.h"
#include "win.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SCREEN_MARGIN 50

typedef struct s_setting {
  char double_click;
  double double_click_delay;
  double last_click;
  char *savefile_path;
  char hs_small[25];
  char hs_medium[25];
  char hs_large[25];
} t_setting;

extern t_setting g_setting;

typedef enum { MAIN, END, SETTING, SCORE, N_MENU } ms_menu;

typedef struct s_minisweep {
  t_menu **menus;
  t_win *win;
  t_grid *grid;
  t_setting sett;
  t_save save_data;
  int alive;
} t_minisweep;

void ms_process_resize(t_minisweep *ms);
void ms_process_input(t_minisweep *ms);
void ms_process_mouseover(t_minisweep *ms);
void ms_draw(t_minisweep *ms);
void ms_destroy(t_minisweep *ms);
t_minisweep *ms_create(void);

#endif
