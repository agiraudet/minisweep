#include "theme.h"

t_theme g_theme;

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
}

void theme_candy() {
  g_theme.bg = PINK;               // Background color
  g_theme.flagged = MAGENTA;       // Flagged cells
  g_theme.hidden = LIGHTGRAY;      // Hidden cells
  g_theme.cell = WHITE;            // Revealed cells
  g_theme.bomb = RED;              // Bombs
  g_theme.timer = MAGENTA;         // Timer
  g_theme.numbers[0] = WHITE;      // Default number color
  g_theme.numbers[1] = BLUE;       // Number 1
  g_theme.numbers[2] = GREEN;      // Number 2
  g_theme.numbers[3] = ORANGE;     // Number 3
  g_theme.numbers[4] = PURPLE;     // Number 4
  g_theme.numbers[5] = GOLD;       // Number 5
  g_theme.numbers[6] = VIOLET;     // Number 6
  g_theme.numbers[7] = DARKPURPLE; // Number 7
  g_theme.numbers[8] = DARKGRAY;   // Number 8
}
void theme_ocean() {
  g_theme.bg = (Color){0, 105, 148, 255};
  g_theme.flagged = (Color){255, 127, 80, 255};
  g_theme.hidden = (Color){224, 255, 255, 255};
  g_theme.cell = (Color){176, 224, 230, 255};
  g_theme.bomb = (Color){255, 69, 0, 255};
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
  void (*f[])(void) = {theme_default, theme_candy, theme_ocean};
  static int i = 0;

  f[i]();

  i = (i + 1) % (sizeof(f) / sizeof(f[0]));
}
