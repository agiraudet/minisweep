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
void theme_next();
int theme_switch(int n);

extern t_theme g_theme;
#endif
