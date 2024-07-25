#include "theme.h"
#include <raylib.h>

// To add a theme, simply creata a new function for it, then add it in the f[]
// array of next_theme()

t_theme g_theme;

Color ultocol(unsigned long color) {
  Color rlColor;
  rlColor.r = (color >> 16) & 0xFF;
  rlColor.g = (color >> 8) & 0xFF;
  rlColor.b = color & 0xFF;
  rlColor.a = 255;
  return rlColor;
}

// Defining theme using Raylib preset colors
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

// Defining theme using hex
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

// Defining theme using RGB
// the Color struct is as follow: {r,g,b,a}
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

void theme_sunset() {
  g_theme.bg = ultocol(0xFF4500);         // Orange Red
  g_theme.flagged = ultocol(0xFF6347);    // Tomato
  g_theme.hidden = ultocol(0xFFD700);     // Gold
  g_theme.cell = ultocol(0xFFA500);       // Orange
  g_theme.bomb = ultocol(0x8B0000);       // Dark Red
  g_theme.timer = ultocol(0xFFFFFF);      // White
  g_theme.numbers[0] = ultocol(0xFFFFFF); // White
  g_theme.numbers[1] = ultocol(0xFF4500); // Orange Red
  g_theme.numbers[2] = ultocol(0xFF6347); // Tomato
  g_theme.numbers[3] = ultocol(0xFFD700); // Gold
  g_theme.numbers[4] = ultocol(0xFFA500); // Orange
  g_theme.numbers[5] = ultocol(0xFF8C00); // Dark Orange
  g_theme.numbers[6] = ultocol(0xFF4500); // Orange Red
  g_theme.numbers[7] = ultocol(0xFF6347); // Tomato
  g_theme.numbers[8] = ultocol(0xFFD700); // Gold
}

void theme_forest() {
  g_theme.bg = ultocol(0x228B22);         // Forest Green
  g_theme.flagged = ultocol(0x8B4513);    // Saddle Brown
  g_theme.hidden = ultocol(0x556B2F);     // Dark Olive Green
  g_theme.cell = ultocol(0x2E8B57);       // Sea Green
  g_theme.bomb = ultocol(0x006400);       // Dark Green
  g_theme.timer = ultocol(0xFFFFFF);      // White
  g_theme.numbers[0] = ultocol(0xFFFFFF); // White
  g_theme.numbers[1] = ultocol(0x00FF00); // Lime
  g_theme.numbers[2] = ultocol(0x32CD32); // Lime Green
  g_theme.numbers[3] = ultocol(0x9ACD32); // Yellow Green
  g_theme.numbers[4] = ultocol(0x6B8E23); // Olive Drab
  g_theme.numbers[5] = ultocol(0x228B22); // Forest Green
  g_theme.numbers[6] = ultocol(0x006400); // Dark Green
  g_theme.numbers[7] = ultocol(0x8B4513); // Saddle Brown
  g_theme.numbers[8] = ultocol(0xA0522D); // Sienna
}

void theme_desert() {
  g_theme.bg = ultocol(0xEDC9AF);         // Desert Sand
  g_theme.flagged = ultocol(0xDAA520);    // Goldenrod
  g_theme.hidden = ultocol(0xDEB887);     // Burly Wood
  g_theme.cell = ultocol(0xD2B48C);       // Tan
  g_theme.bomb = ultocol(0x8B4513);       // Saddle Brown
  g_theme.timer = ultocol(0xFFFFFF);      // White
  g_theme.numbers[0] = ultocol(0xFFFFFF); // White
  g_theme.numbers[1] = ultocol(0xDAA520); // Goldenrod
  g_theme.numbers[2] = ultocol(0xD2691E); // Chocolate
  g_theme.numbers[3] = ultocol(0xCD853F); // Peru
  g_theme.numbers[4] = ultocol(0xF4A460); // Sandy Brown
  g_theme.numbers[5] = ultocol(0xA0522D); // Sienna
  g_theme.numbers[6] = ultocol(0x8B4513); // Saddle Brown
  g_theme.numbers[7] = ultocol(0xCD853F); // Peru
  g_theme.numbers[8] = ultocol(0xDEB887); // Burly Wood
}

void theme_next() { theme_switch(-2); }

int theme_switch(int n) {
  void (*f[])(void) = {theme_default, theme_kandy,  theme_ocean,
                       theme_sunset,  theme_forest, theme_desert};
  static int i = 0;
  if (n == -1) {
    return i;
  } else if (n == -2) {
    i = (i + 1) % (sizeof(f) / sizeof(f[0]));
    f[i]();
  } else if (n < (int)(sizeof(f) / sizeof(f[0]))) {
    i = n;
    f[i]();
  }
  return i;
}
