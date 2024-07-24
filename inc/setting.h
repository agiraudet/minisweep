#ifndef SETTING_H
#define SETTING_H

typedef struct s_setting {
  char double_click;
  double double_click_delay;
  double last_click;
} t_setting;

extern t_setting g_setting;

#endif
