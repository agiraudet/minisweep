#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stddef.h>

typedef struct s_button {
  Rectangle rect;
  Color *color;
  float round;
  float border_w;
  Color *border_color;
  Color *label_color;
  char *label;
} t_button;

typedef struct s_menu {
  Color *bg;
  char dir;
  t_button *butlst;
  int x;
  int y;
  size_t nbut;
  int win_w;
  int win_h;
  int pan;
  char *title;
  char *subtitle;
  int border_w;
  int draggable;
  int dragged;
  int dragX;
  int dragY;
} t_menu;

t_menu *menu_create_main(size_t winw, size_t winh);
void menu_draw(t_menu *mn);
const char *menu_find_clic(t_menu *mn, int x, int y);
const char *menu_find_dragorclic(t_menu *mn, int x, int y);
void menu_destroy(t_menu *mn);
void menu_update_size(t_menu *mn, size_t w, size_t h);
t_menu *menu_create_end(size_t winw, size_t winh);
void menu_mov(t_menu *mn, int x, int y);
void menu_movdrag(t_menu *mn);
void menu_undrag(t_menu *mn);
void menu_setsubtitle(t_menu *mn, const char *new_title);
void menu_settitle(t_menu *mn, const char *new_title);

#endif
