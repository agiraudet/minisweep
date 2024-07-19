#ifndef THEME_H
#define THEME_H

#include <raylib.h>

typedef struct s_theme {

  Color bg;
  Color flagged;
  Color hidden;
  Color bomb;
  Color cell;
  Color timer;
  Color numbers[9];

} t_theme;

void theme_default();
void next_theme();

extern t_theme g_theme;
#endif
