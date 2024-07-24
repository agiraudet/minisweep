#ifndef SAVE_H
#define SAVE_H

typedef struct s_save {
  char *file_path;
  double hs_small;
  double hs_medium;
  double hs_large;
  char double_clic;
} t_save;

void save_init(t_save *save_data);
int save_load(t_save *save_data);
int save_write(t_save *save_data);

#endif
