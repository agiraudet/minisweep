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
  const char *config_dir = SAVE_DIR;
  const char *file_name = SAVE_FILE;
  size_t path_len =
      strlen(home_dir) + strlen(config_dir) + strlen(file_name) + 1;
  char *full_path = (char *)malloc(path_len);
  snprintf(full_path, path_len, "%s%s%s", home_dir, config_dir, file_name);
  return full_path;
}

void save_init(t_save *save_data) {
  if (!save_data)
    return;
  save_data->version = SAVE_VERSION;
  save_data->theme_n = -1;
  save_data->hs_small = 0;
  save_data->hs_medium = 0;
  save_data->hs_large = 0;
  save_data->double_clic = 0;
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

int save_writeraw(t_save *save_data, const char *file_path) {
  if (!save_data)
    return -1;
  char *dir_path = strdup(file_path);
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
  FILE *file = fopen(file_path, "wb");
  if (file == NULL) {
    perror("Failed to open save file for writing");
    return -1;
  }

  if (fwrite(save_data, sizeof(t_save), 1, file) != 1) {
    perror("Failed to write save data");
    fclose(file);
    return -1;
  }

  fclose(file);
  return 0;
}

int save_loadraw(t_save *save_data, const char *file_path) {
  if (!save_data)
    return -1;
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    return -1;
  }
  if (fread(save_data, sizeof(t_save), 1, file) != 1) {
    perror("Failed to read save data");
    fclose(file);
    return -1;
  }
  fclose(file);
  if (save_data->version != SAVE_VERSION)
    printf("Warning! Save file verison does not match binary\n");
  return 0;
}
