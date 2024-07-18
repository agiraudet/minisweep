#include "menu.h"
#include "theme.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void button_init(button *but, const char *label) {
  if (!but)
    return;
  but->rect.width = 250;
  but->rect.height = 50;
  but->rect.x = 50;
  but->rect.y = 50;
  but->color = LIGHTGRAY;
  but->label = strdup(label);
  but->round = 0;
  but->border_color = WHITE;
  but->border_w = 0;
  but->label_color = BLACK;
}

void button_drawlabel(button *but) {
  int fontSize = but->rect.height / 2;
  int labelX =
      but->rect.x + but->rect.width / 2 - MeasureText(but->label, fontSize) / 2;
  int labelY = but->rect.y + but->rect.height / 2 - fontSize / 2;
  DrawText(but->label, labelX, labelY, fontSize, but->label_color);
}

void button_draw(button *but) {
  if (but->round) {
    DrawRectangleRounded(but->rect, but->round, 4, but->color);
    if (but->border_w)
      DrawRectangleRoundedLines(but->rect, but->round, 4, but->border_w,
                                but->border_color);
  } else {
    DrawRectangleRec(but->rect, but->color);
    if (but->border_w)
      DrawRectangleLinesEx(but->rect, but->border_w, but->border_color);
  }
  button_drawlabel(but);
}

void menu_update_pos(menu *mn) {
  mn->pan = mn->win_h / 3;
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    but->rect.height = mn->pan / (mn->nbut * 2 - 1);
    but->rect.x = mn->win_w / 2 - but->rect.width / 2;
    but->rect.y =
        mn->pan + but->rect.height * i + (i > 0 ? but->rect.height * i : 0);
  }
}

void menu_update_size(menu *mn, size_t w, size_t h) {
  mn->win_w = w;
  mn->win_h = h;
  menu_update_pos(mn);
}

menu *menu_create_main(size_t winw, size_t winh) {
  const char *labels[] = {"small", "medium", "large", "change theme"};

  menu *mn = malloc(sizeof(menu));
  mn->win_w = winw;
  mn->win_h = winh;
  mn->nbut = 4;
  mn->butlst = malloc(sizeof(button) * mn->nbut);
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    button_init(&mn->butlst[i], labels[i]);
    but->round = 0.2;
    but->border_w = 2;
  }
  menu_update_pos(mn);
  menu_update_colors(mn);
  return mn;
}

void menu_draw_title(menu *mn) {
  const char title[] = "MiniSweep";
  int fontSize = (mn->win_w) / strlen(title);
  int posX = mn->win_w / 2 - MeasureText(title, fontSize) / 2;
  DrawText(title, posX, mn->pan / 2 - fontSize / 2, fontSize, g_theme.timer);
}

void menu_draw(menu *mn) {
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    button_draw(but);
  }
  menu_draw_title(mn);
}

void menu_update_colors(menu *mn) {
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    but->color = g_theme.cell;
    but->border_color = g_theme.timer;
    but->label_color = g_theme.timer;
  }
}

const char *menu_find_clic(menu *mn, int x, int y) {
  Vector2 clic;
  clic.x = x;
  clic.y = y;
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    if (CheckCollisionPointRec(clic, but->rect))
      return but->label;
  }
  return 0;
}

void menu_destroy(menu *mn) {
  if (!mn)
    return;
  for (size_t i = 0; i < mn->nbut; i++) {
    button *but = &mn->butlst[i];
    if (!but)
      continue;
    free(but->label);
  }
  free(mn->butlst);
  free(mn);
}
