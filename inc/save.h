#ifndef SAVE_H
#define SAVE_H

#define SAVE_VERSION 1
#define SAVE_DIR "/.config/minisweep/"
#define SAVE_FILE "save"

typedef struct s_save {
  int version;
  double hs_small;
  double hs_medium;
  double hs_large;
  char double_clic;
} t_save;

char *save_getpath(void);
void save_init(t_save *save_data);
int save_writeraw(t_save *save_data, const char *file_path);
int save_loadraw(t_save *save_data, const char *file_path);

#endif
