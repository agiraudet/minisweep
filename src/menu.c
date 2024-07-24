#include "menu.h"
#include "theme.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void button_init(t_button *but, const char *label) {
  if (!but)
    return;
  but->rect.width = 250;
  but->rect.height = 50;
  but->rect.x = 50;
  but->rect.y = 50;
  but->color = &g_theme.cell;
  but->label = strdup(label);
  but->round = 0;
  but->border_color = &g_theme.timer;
  but->border_w = 0;
  but->label_color = &g_theme.timer;
}

void button_drawlabel(t_button *but) {
  int fontSize = but->rect.height / 2;
  int labelX = but->rect.x + but->rect.width / 2 -
               (float)MeasureText(but->label, fontSize) / 2;
  int labelY = but->rect.y + but->rect.height / 2 - (float)fontSize / 2;
  DrawText(but->label, labelX, labelY, fontSize, *but->label_color);
}

void button_draw(t_button *but) {
  if (but->round) {
    DrawRectangleRounded(but->rect, but->round, 4, *but->color);
    if (but->border_w)
      DrawRectangleRoundedLines(but->rect, but->round, 4, but->border_w,
                                *but->border_color);
  } else {
    DrawRectangleRec(but->rect, *but->color);
    if (but->border_w)
      DrawRectangleLinesEx(but->rect, but->border_w, *but->border_color);
  }
  button_drawlabel(but);
}

void menu_update_pos(t_menu *mn) {
  if (mn->dir == 'v') {
    mn->pan = mn->win_h / 3;
    for (size_t i = 0; i < mn->nbut; i++) {
      t_button *but = &mn->butlst[i];
      but->rect.height = (float)mn->pan / (mn->nbut * 2 - 1);
      but->rect.x = (float)mn->win_w / 2 - but->rect.width / 2 + mn->x;
      but->rect.y = mn->y + mn->pan + but->rect.height * i +
                    (i > 0 ? but->rect.height * i : 0);
    }
  } else {
    mn->pan = mn->win_w / 5;
    for (size_t i = 0; i < mn->nbut; i++) {
      t_button *but = &mn->butlst[i];
      but->rect.width = (float)(mn->win_w - mn->pan * 2) / (mn->nbut * 2 - 1);
      but->rect.x = mn->x + mn->pan + but->rect.width * i +
                    (i > 0 ? but->rect.width * i : 0);
      but->rect.y = (float)mn->win_h / 2 - but->rect.height / 2 + mn->y;
    }
  }
}

void menu_update_size(t_menu *mn, size_t w, size_t h) {
  mn->win_w = w;
  mn->win_h = h;
  menu_update_pos(mn);
}

void menu_draw_title(t_menu *mn) {
  if (!mn || !mn->title)
    return;
  const char *title = mn->title;
  int fontSize = (mn->win_w) / strlen(title);
  int posX = mn->x + mn->win_w / 2 - MeasureText(title, fontSize) / 2;
  DrawText(title, posX, mn->pan / 2 - fontSize / 2 + mn->y, fontSize,
           g_theme.timer);
}

void menu_draw_subtitle(t_menu *mn) {
  if (!mn || !mn->subtitle)
    return;
  const char *title = mn->subtitle;
  int fontSize = (mn->win_w) / strlen(title) / 2;
  int posX = mn->x + mn->win_w / 2 - MeasureText(title, fontSize) / 2;
  DrawText(title, posX, mn->y + mn->win_h - (mn->pan / 2 + fontSize / 2),
           fontSize, g_theme.timer);
}

void menu_draw(t_menu *mn) {
  DrawRectangle(mn->x, mn->y, mn->win_w, mn->win_h, *mn->bg);
  if (mn->border_w) {
    Color color = g_theme.timer;
    DrawRectangle(mn->x, mn->y, mn->win_w, mn->border_w, color);
    DrawRectangle(mn->x, mn->y + mn->win_h - mn->border_w, mn->win_w,
                  mn->border_w, color);
    DrawRectangle(mn->x, mn->y, mn->border_w, mn->win_h, color);
    DrawRectangle(mn->x + mn->win_w - mn->border_w, mn->y, mn->border_w,
                  mn->win_h, color);
  }
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    button_draw(but);
  }
  menu_draw_title(mn);
  menu_draw_subtitle(mn);
}

void menu_mov(t_menu *mn, int x, int y) {
  mn->x = x;
  mn->y = y;
  menu_update_pos(mn);
}

void menu_movdrag(t_menu *mn) {
  if (mn->draggable && mn->dragged) {
    int x = GetMouseX() - mn->dragX;
    int y = GetMouseY() - mn->dragY;
    menu_mov(mn, x, y);
  }
}

const char *menu_find_clic(t_menu *mn, int x, int y) {
  Vector2 clic;
  clic.x = x;
  clic.y = y;
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    if (CheckCollisionPointRec(clic, but->rect))
      return but->label;
  }
  return 0;
}

const char *menu_find_dragorclic(t_menu *mn, int x, int y) {
  Vector2 clic;
  clic.x = x;
  clic.y = y;

  Rectangle bar = {mn->x, mn->y, mn->win_w, mn->border_w};
  if (CheckCollisionPointRec(clic, bar)) {
    if (mn->draggable) {
      if (!mn->dragged) {
        mn->dragX = x - mn->x;
        mn->dragY = y - mn->y;
      }
      mn->dragged = 1;
    } else
      mn->dragged = 0;
    return 0;
  }
  return menu_find_clic(mn, x, y);
}

void menu_undrag(t_menu *mn) { mn->dragged = 0; }

void menu_destroy(t_menu *mn) {
  if (!mn)
    return;
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    if (!but)
      continue;
    free(but->label);
  }
  free(mn->butlst);
  if (mn->title)
    free(mn->title);
  if (mn->subtitle)
    free(mn->subtitle);
  free(mn);
}

void menu_settitle(t_menu *mn, const char *new_title) {
  if (mn->title)
    free(mn->title);
  mn->title = strdup(new_title);
}

void menu_setsubtitle(t_menu *mn, const char *new_title) {
  if (mn->subtitle)
    free(mn->subtitle);
  mn->subtitle = strdup(new_title);
}

t_menu *menu_create_main(size_t winw, size_t winh) {
  const char *labels[] = {"small", "medium", "large", "change theme"};

  t_menu *mn = malloc(sizeof(t_menu));
  mn->visible = 1;
  mn->dragged = 0;
  mn->draggable = 0;
  mn->dragX = 0;
  mn->dragY = 0;
  mn->bg = &g_theme.bg;
  mn->border_w = 0;
  mn->x = 0;
  mn->y = 0;
  mn->title = strdup("MiniSweep");
  mn->subtitle = 0;
  mn->dir = 'v';
  mn->win_w = winw;
  mn->win_h = winh;
  mn->nbut = 4;
  mn->butlst = malloc(sizeof(t_button) * mn->nbut);
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    button_init(&mn->butlst[i], labels[i]);
    but->round = 0.2;
    but->border_w = 2;
  }
  menu_update_pos(mn);
  return mn;
}

t_menu *menu_create_end(size_t winw, size_t winh) {
  const char *labels[] = {"menu", "quit"};
  t_menu *mn = malloc(sizeof(t_menu));
  mn->visible = 0;
  mn->dragged = 0;
  mn->draggable = 1;
  mn->dragX = 0;
  mn->dragY = 0;
  mn->bg = &g_theme.bg;
  mn->border_w = 10;
  mn->x = 100;
  mn->y = 100;
  mn->title = strdup("Finished");
  mn->subtitle = strdup("test");
  mn->dir = 'h';
  mn->win_w = winw;
  mn->win_h = winh;
  mn->nbut = 2;
  mn->butlst = malloc(sizeof(t_button) * mn->nbut);
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    button_init(&mn->butlst[i], labels[i]);
    but->round = 0.2;
    but->border_w = 2;
  }
  menu_update_pos(mn);
  return mn;
}

t_menu *menu_create_setting(size_t winw, size_t winh) {
  const char *labels[] = {"enable double click", "cancel"};
  t_menu *mn = malloc(sizeof(t_menu));
  mn->visible = 0;
  mn->dragged = 0;
  mn->draggable = 1;
  mn->dragX = 0;
  mn->dragY = 0;
  mn->bg = &g_theme.bg;
  mn->border_w = 10;
  mn->x = 100;
  mn->y = 100;
  mn->title = strdup("Settings");
  mn->subtitle = 0;
  mn->dir = 'v';
  mn->win_w = winw;
  mn->win_h = winh;
  mn->nbut = 2;
  mn->butlst = malloc(sizeof(t_button) * mn->nbut);
  for (size_t i = 0; i < mn->nbut; i++) {
    t_button *but = &mn->butlst[i];
    button_init(&mn->butlst[i], labels[i]);
    but->round = 0.2;
    but->border_w = 2;
  }
  menu_update_pos(mn);
  return mn;
}
