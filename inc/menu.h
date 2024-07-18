#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stddef.h>

typedef struct button {
  Rectangle rect;
  Color color;
  float round;
  float border_w;
  Color border_color;
  Color label_color;
  char *label;
} button;

typedef struct menu {
  button *butlst;
  size_t nbut;
  size_t win_w;
  size_t win_h;
  size_t pan;
} menu;

void button_init(button *but, const char *label);
void button_drawlabel(button *but);
void button_draw(button *but);
void menu_update_pos(menu *mn);
void menu_update_size(menu *mn, size_t w, size_t h);
menu *menu_create_main(size_t winw, size_t winh);
void menu_draw(menu *mn);
const char *menu_find_clic(menu *mn, int x, int y);
void menu_destroy(menu *mn);
void menu_update_colors(menu *mn);

#endif
