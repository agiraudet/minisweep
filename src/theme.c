#include "theme.h"
#include <raylib.h>

t_theme g_theme;

Color ultocol(unsigned long color) {
  Color rlColor;
  rlColor.r = (color >> 16) & 0xFF;
  rlColor.g = (color >> 8) & 0xFF;
  rlColor.b = color & 0xFF;
  rlColor.a = 255;
  return rlColor;
}

void theme_default() {
  g_theme.bg = BLACK;
  g_theme.flagged = YELLOW;
  g_theme.hidden = DARKGRAY;
  g_theme.cell = GRAY;
  g_theme.bomb = RED;
  g_theme.timer = WHITE;
  g_theme.numbers[0] = WHITE;
  g_theme.numbers[1] = DARKBLUE;
  g_theme.numbers[2] = DARKGREEN;
  g_theme.numbers[3] = RED;
  g_theme.numbers[4] = PURPLE;
  g_theme.numbers[5] = ORANGE;
  g_theme.numbers[6] = MAROON;
  g_theme.numbers[7] = LIME;
  g_theme.numbers[8] = BROWN;
}

void theme_kandy() {
  g_theme.bg = ultocol(0xFF7199);
  g_theme.flagged = ultocol(0x52E595);
  g_theme.hidden = ultocol(0xFFE2EA);
  g_theme.cell = ultocol(0xFFBADA0);
  g_theme.bomb = ultocol(0xFF6533);
  g_theme.timer = ultocol(0xF5F5F5);
  g_theme.numbers[0] = ultocol(0xFFFFFF);
  g_theme.numbers[1] = ultocol(0x278DFB);
  g_theme.numbers[2] = ultocol(0x27FB68);
  g_theme.numbers[3] = ultocol(0xD0D800);
  g_theme.numbers[4] = ultocol(0xC100D8);
  g_theme.numbers[5] = ultocol(0x00D8A1);
  g_theme.numbers[6] = ultocol(0x06D800);
  g_theme.numbers[7] = ultocol(0x06D800);
  g_theme.numbers[8] = ultocol(0x06D800);
}

void theme_ocean() {
  g_theme.bg = (Color){0, 105, 148, 255};
  g_theme.flagged = (Color){255, 127, 80, 255};
  g_theme.hidden = (Color){224, 255, 255, 255};
  g_theme.cell = (Color){176, 224, 230, 255};
  g_theme.bomb = (Color){255, 10, 0, 255};
  g_theme.timer = (Color){255, 255, 224, 255};
  g_theme.numbers[0] = (Color){255, 255, 255, 255};
  g_theme.numbers[1] = (Color){0, 191, 255, 255};
  g_theme.numbers[2] = (Color){34, 139, 34, 255};
  g_theme.numbers[3] = (Color){255, 69, 0, 255};
  g_theme.numbers[4] = (Color){153, 50, 204, 255};
  g_theme.numbers[5] = (Color){255, 140, 0, 255};
  g_theme.numbers[6] = (Color){72, 61, 139, 255};
  g_theme.numbers[7] = (Color){75, 0, 130, 255};
  g_theme.numbers[8] = (Color){47, 79, 79, 255};
}

void next_theme() {
  void (*f[])(void) = {theme_default, theme_kandy, theme_ocean};
  static int i = 0;

  f[i]();

  i = (i + 1) % (sizeof(f) / sizeof(f[0]));
}
