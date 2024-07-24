#include "save.h"

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *save_getpath(void) {
  const char *home_dir = getenv("HOME");
  if (home_dir == NULL) {
    home_dir = getpwuid(getuid())->pw_dir;
  }
  const char *config_dir = "/.config/minisweep/";
  const char *file_name = "file";
  size_t path_len =
      strlen(home_dir) + strlen(config_dir) + strlen(file_name) + 1;
  char *full_path = (char *)malloc(path_len);
  snprintf(full_path, path_len, "%s%s%s", home_dir, config_dir, file_name);
  return full_path;
}

void save_init(t_save *save_data) {
  if (!save_data)
    return;
  save_data->file_path = save_getpath();
  save_data->hs_small = 0;
  save_data->hs_medium = 0;
  save_data->hs_large = 0;
  save_data->double_clic = 0;
}

int save_load(t_save *save_data) {
  if (!save_data)
    return -1;
  FILE *file = fopen(save_data->file_path, "r");
  if (file == NULL) {
    return -1;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "hs_small=", 9) == 0) {
      save_data->hs_small = atof(line + 9);
    } else if (strncmp(line, "hs_medium=", 10) == 0) {
      save_data->hs_medium = atof(line + 10);
    } else if (strncmp(line, "hs_large=", 9) == 0) {
      save_data->hs_large = atof(line + 9);
    } else if (strncmp(line, "double_clic=", 12) == 0) {
      save_data->double_clic = atoi(line + 12);
    }
  }
  fclose(file);
  return 0;
}

int save_create_directories(const char *path) {
  char temp[256];
  char *pos = NULL;
  size_t len;
  snprintf(temp, sizeof(temp), "%s", path);
  len = strlen(temp);
  if (temp[len - 1] == '/')
    temp[len - 1] = 0;
  for (pos = temp + 1; *pos; pos++) {
    if (*pos == '/') {
      *pos = 0;
      if (mkdir(temp, S_IRWXU) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        return -1;
      }
      *pos = '/';
    }
  }
  if (mkdir(temp, S_IRWXU) != 0 && errno != EEXIST) {
    perror("Failed to create directory");
    return -1;
  }
  return 0;
}

int save_write(t_save *save_data) {
  if (!save_data)
    return -1;
  char *dir_path = strdup(save_data->file_path);
  if (dir_path == NULL)
    return -1;

  char *last_slash = strrchr(dir_path, '/');
  if (last_slash != NULL) {
    *last_slash = '\0';
    if (save_create_directories(dir_path) != 0) {
      free(dir_path);
      return -1;
    }
  }
  free(dir_path);
  FILE *file = fopen(save_data->file_path, "w");
  if (file == NULL) {
    perror("Failed to open save file for writing");
    return -1;
  }
  fprintf(file, "hs_small=%.2f\n", save_data->hs_small);
  fprintf(file, "hs_medium=%.2f\n", save_data->hs_medium);
  fprintf(file, "hs_large=%.2f\n", save_data->hs_large);
  fprintf(file, "double_clic=%d\n", save_data->double_clic);
  fclose(file);
  return 0;
}
