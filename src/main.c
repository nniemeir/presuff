#include "presuff.h"

void print_usage(void) {
  printf("Usage: presuff [options]\n");
  printf("Options:\n");
  printf("  -a               Append pattern to basenames\n");
  printf("  -b               Prepend pattern to basenames\n");
  printf("  -d <directory>   Specify directory\n");
  printf("  -h               Display this help message\n");
  printf("  -p <pattern>     Specify pattern to prepend/append\n");
  printf("  -x <extension>   Specify extension of files to perform "
         "renaming on\n");
  printf("\n");
  printf("Example usage:\n");
  printf("  presuff -b -d /path/to/directory -p pat_ -x txt\n");
  printf("  presuff -a -p _pat\n");
  printf("\n");
}

void validate_mode_selection(const int mode_args_given) {
  if (mode_args_given == 0) {
    printf("Missing mode argument.\n");
    exit(EXIT_FAILURE);
  }

  if (mode_args_given == 2) {
    printf("Conflicting mode arguments given.\n");
    exit(EXIT_FAILURE);
  }
}

void process_args(int argc, char *argv[], struct args *current_args) {
  opterr = 0;
  int mode_args_given = 0;
  int c;

  while ((c = getopt(argc, argv, "abd:hx:p:")) != -1) {
    switch (c) {
    case 'a':
      mode_args_given++;
      current_args->mode = MODE_APPEND;
      break;
    case 'b':
      mode_args_given++;
      current_args->mode = MODE_PREPEND;
      break;
    case 'd':
      snprintf(current_args->dir_path, PATH_MAX, "%s", optarg);
      break;
    case 'h':
      print_usage();
      exit(EXIT_SUCCESS);
    case 'p':
      snprintf(current_args->pattern, NAME_MAX, "%s", optarg);
      break;
    case 'x':
      snprintf(current_args->filtered_extension, NAME_MAX, "%s", optarg);
      break;
    case '?':
      fprintf(stderr, "Unknown option '-%c'. Run with -h for options.\n",
              optopt);
      exit(EXIT_FAILURE);
    }
  }

  validate_mode_selection(mode_args_given);

  if (current_args->dir_path[0] == '\0') {
    if (!getcwd(current_args->dir_path, PATH_MAX)) {
      fprintf(stderr, "Failed to assign working directory to variable");
      exit(EXIT_FAILURE);
    }
  }

  if (current_args->filtered_extension[0] == '\0') {
    return;
  }
}

int main(int argc, char *argv[]) {
  struct args current_args;

  memset(&current_args, 0, sizeof(current_args));

  process_args(argc, argv, &current_args);

  if (pattern_rename(&current_args) == 1) {
    fprintf(stderr, "Renaming files failed.\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
