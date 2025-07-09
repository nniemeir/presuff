#include "presuff.h"
#include <errno.h>

char *construct_original_file_path(struct args *current_args,
                                   const char *filename) {
  size_t dir_path_length = strlen(current_args->dir_path);
  size_t filename_length = strlen(filename);

  char *file_path = malloc(dir_path_length + SLASH_LENGTH + filename_length +
                           NULL_TERMINATOR_LENGTH);
  if (!file_path) {
    fprintf(stderr, "Error allocating memory for file_path: %s",
            strerror(errno));
    return NULL;
  }

  snprintf(file_path,
           dir_path_length + SLASH_LENGTH + filename_length +
               NULL_TERMINATOR_LENGTH,
           "%s/%s", current_args->dir_path, filename);

  return file_path;
}

char *construct_new_file_path(struct args *current_args, const char *filename) {
  char *file_extension = strrchr(filename, '.');

  if (!file_extension) {
    return NULL;
  }

  size_t dir_path_length = strlen(current_args->dir_path);
  size_t pattern_length = strlen(current_args->pattern);
  size_t file_extension_length = strlen(file_extension);
  size_t basename_length = strlen(filename) - file_extension_length;

  char *basename = malloc(basename_length + NULL_TERMINATOR_LENGTH);
  if (!basename) {
    fprintf(stderr, "Error allocating memory for basename: %s",
            strerror(errno));
    return NULL;
  }

  strncpy(basename, filename, basename_length);
  basename[basename_length] = '\0';

  size_t total_length = dir_path_length + SLASH_LENGTH + pattern_length +
                        basename_length + file_extension_length +
                        NULL_TERMINATOR_LENGTH;

  char *new_file_path = malloc(total_length);
  if (!new_file_path) {
    fprintf(stderr, "Error allocating memory for new_file_path: %s",
            strerror(errno));
    return NULL;
  }

  if (current_args->mode == MODE_PREPEND) {
    snprintf(new_file_path, total_length, "%s/%s%s", current_args->dir_path,
             current_args->pattern, filename);
  }

  if (current_args->mode == MODE_APPEND) {
    snprintf(new_file_path, total_length, "%s/%s%s%s", current_args->dir_path,
             basename, current_args->pattern, file_extension);
  }

  free(basename);

  return new_file_path;
}

void rename_file(const char *original_file_path, const char *new_file_path) {
  if (strlen(new_file_path) >= PATH_MAX) {
    fprintf(stderr, "Maximum filename length exceeded for %s\n",
            original_file_path);
    return;
  }

  if (rename(original_file_path, new_file_path) != 0) {
    fprintf(stderr, "Error renaming file %s to %s\n", original_file_path,
            new_file_path);
  }
}

int pattern_rename(struct args *current_args) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(current_args->dir_path);
  if (!dir) {
    fprintf(stderr, "Error opening directory %s: %s", current_args->dir_path,
            strerror(errno));
    return 1;
  }

  while ((entry = readdir(dir))) {
    // Ignore non-files
    if (entry->d_type != DT_REG) {
      continue;
    }

    char *file_extension = strrchr(entry->d_name, '.');
    // Ignore files without extensions
    if (!file_extension || file_extension[0] == '\0') {
      continue;
    }

    file_extension++;

    // Ignore files without the filtered extension if one has been specified
    if (strcmp(file_extension, current_args->filtered_extension) != 0 &&
        current_args->filtered_extension[0] != '\0') {
      continue;
    }

    char *original_file_path =
        construct_original_file_path(current_args, entry->d_name);

    char *new_file_path = construct_new_file_path(current_args, entry->d_name);

    if (new_file_path) {
      rename_file(original_file_path, new_file_path);
    }

    free(original_file_path);

    free(new_file_path);
  }

  if (closedir(dir) == -1) {
    fprintf(stderr, "Failed to close directory %s: %s", current_args->dir_path,
            strerror(errno));
    return 1;
  }

  return 0;
}
