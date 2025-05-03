// Author: Jacob Niemeir <nniemeir@protonmail.com>
// Prepend or append a string to each filename of the chosen extension in the
// chosen directory
#include "../include/presuff.h"

void process_args(int argc, char *argv[], char *dir_arg, char *pattern,
                  char *ext_arg, char *filtered_extension, char *dir_path,
                  int *mode) {
  opterr = 0;
  int mode_args_given = 0;
  int index, c;
  while ((c = getopt(argc, argv, "b::d:e::h::x:p:")) != -1) {
    switch (c) {
    case 'b':
      mode_args_given++;
      *mode = MODE_PREPEND;
      break;
    case 'd':
      if (!optarg) {
        fprintf(stderr, "Option -d requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(dir_arg, PATH_MAX, "%s", optarg);
      break;
    case 'e':
      mode_args_given++;
      *mode = MODE_APPEND;
      break;
    case 'h':
      printf("Usage: presuff [options]\n");
      printf("Options:\n");
      printf("  -b               Prepend pattern to basenames\n");
      printf("  -d <directory>   Specify directory\n");
      printf("  -e               Append pattern to basenames\n");
      printf("  -h               Display this help message\n");
      printf("  -p <pattern>     Specify pattern to prepend/append\n");
      printf("  -x <extension>   Specify extension of files to perform "
             "renaming on\n");
      printf("\n");
      printf("Example usage:\n");
      printf("  presuff -b -d /path/to/directory -p pat_ -x txt\n");
      printf("  presuff -e -p _pat\n");
      printf("\n");
      exit(EXIT_SUCCESS);
    case 'p':
      if (!optarg) {
        fprintf(stderr, "Option -p requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(pattern, NAME_MAX, "%s", optarg);
      break;
    case 'x':
      if (!optarg) {
        fprintf(stderr, "Option -x requires an argument.\n");
        exit(EXIT_FAILURE);
      }
      snprintf(ext_arg, NAME_MAX, "%s", optarg);
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }
  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  if (!mode_args_given) {
    printf("Missing mode argument.\n");
    exit(EXIT_FAILURE);
  }

  if (mode_args_given == 2) {
    printf("Conflicting mode arguments given.\n");
    exit(EXIT_FAILURE);
  }

  if (dir_arg[0] == '\0') {
    if (!getcwd(dir_path, PATH_MAX)) {
      fprintf(stderr, "Failed to assign working directory to variable");
      exit(EXIT_FAILURE);
    }
  } else {
    snprintf(dir_path, PATH_MAX, "%s", dir_arg);
  }

  if (ext_arg[0] == '\0') {
    snprintf(filtered_extension, sizeof("None"), "None");
  } else {
    snprintf(filtered_extension, sizeof(ext_arg), "%s", ext_arg);
  }
}

int main(int argc, char *argv[]) {
  char dir_path[PATH_MAX] = {0};
  char dir_arg[PATH_MAX] = {0};
  char filtered_extension[NAME_MAX] = {0};
  char pattern[NAME_MAX] = {0};
  char ext_arg[NAME_MAX] = {0};
  int mode = 0;

  process_args(argc, argv, dir_arg, pattern, ext_arg, filtered_extension,
               dir_path, &mode);

  if (!pattern_rename(dir_path, pattern, filtered_extension, mode)) {
    fprintf(stderr, "Renaming files failed.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}