#include "../include/presuff.h"

// Copy given directory path and original filename to a new string
char *construct_original_file_path(const char *dir_path, const char *filename) {
  char *file_path = malloc(strlen(dir_path) + SLASH_LENGTH + strlen(filename) +
                           NULL_TERMINATOR_LENGTH);
  if (file_path == NULL) {
    fprintf(stderr, "Error allocating memory for file path");
    return NULL;
  }
  snprintf(file_path,
           strlen(dir_path) + SLASH_LENGTH + strlen(filename) +
               NULL_TERMINATOR_LENGTH,
           "%s/%s", dir_path, filename);
  return file_path;
}

char *construct_new_file_path(const char *dir_path, const char *pattern,
                              const char *filename, long mode) {

  // The file extension occurs after the last period in the filename
  char *file_extension = strrchr(filename, *dot);
  if (file_extension == NULL || file_extension[0] == '\0') {
    return NULL;
  }

  // Copy the basename to a new string
  size_t basename_length = strlen(filename) - strlen(file_extension);
  char *basename = malloc(basename_length + NULL_TERMINATOR_LENGTH);
  if (basename == NULL) {
    fprintf(stderr, "Error allocating memory for basename");
    return NULL;
  }
  snprintf(basename, basename_length + NULL_TERMINATOR_LENGTH, "%s", filename);

  // Get the size of each of the file path's components
  size_t dir_path_length = strlen(dir_path);
  size_t pattern_length = strlen(pattern);
  size_t file_extension_length = strlen(file_extension);

  char *new_file_path;
  size_t total_length = dir_path_length + SLASH_LENGTH + pattern_length +
                        basename_length + file_extension_length +
                        NULL_TERMINATOR_LENGTH;
  new_file_path = malloc(total_length);
  if (new_file_path == NULL) {
    fprintf(stderr, "Error allocating memory for new file path");
    return NULL;
  }

  if (mode == MODE_PREPEND) {
    snprintf(new_file_path,
             strlen(dir_path) + SLASH_LENGTH + strlen(pattern) +
                 basename_length + strlen(file_extension) +
                 NULL_TERMINATOR_LENGTH,
             "%s/%s%s", dir_path, pattern, filename);
  }

  if (mode == MODE_APPEND) {
    snprintf(new_file_path,
             strlen(dir_path) + SLASH_LENGTH + strlen(pattern) +
                 basename_length + strlen(file_extension) +
                 NULL_TERMINATOR_LENGTH,
             "%s/%s%s%s", dir_path, basename, pattern, file_extension);
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

int pattern_rename(const char *dir_path, const char *pattern,
                   const char *filtered_extension, long mode) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(dir_path);
  if (dir == NULL) {
    fprintf(stderr, "Error opening directory: %s\n", dir_path);
    return 0;
  }

  while ((entry = readdir(dir))) {
    // Ignore non-files
    if (entry->d_type != DT_REG) {
      continue;
    }

    char *file_extension = strrchr(entry->d_name, *dot);
    // Ignore files without extensions
    if (!file_extension || file_extension[0] == '\0') {
      continue;
    }
    file_extension++;

    // Ignore files without the filtered extension if one has been specified
    size_t extension_length = strlen(file_extension) + NULL_TERMINATOR_LENGTH;
    char modified_extension[extension_length];
    snprintf(modified_extension, extension_length, "%s", file_extension);
    if (strcmp(modified_extension, filtered_extension) == 1 ||
        strcmp(filtered_extension, "None") == 1) {
      continue;
    }

    // Set the new file path based on the note
    char *original_file_path =
        construct_original_file_path(dir_path, entry->d_name);
    char *new_file_path =
        construct_new_file_path(dir_path, pattern, entry->d_name, mode);
    if (new_file_path) {
      rename_file(original_file_path, new_file_path);
    }
    free(original_file_path);
    free(new_file_path);
  }

  closedir(dir);
  return 1;
}