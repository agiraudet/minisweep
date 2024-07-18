#ifndef THEME_H
#define THEME_H

#include <raylib.h>

typedef struct s_theme {

  Color bg;
  Color numbers[6];
  Color flagged;
  Color hidden;
  Color bomb;
  Color cell;
  Color timer;

} t_theme;

void theme_default();
void theme_candy();

void next_theme();

extern t_theme g_theme;
#endif
