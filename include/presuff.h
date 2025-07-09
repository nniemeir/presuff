#ifndef PRESUFF_H_
#define PRESUFF_H_

#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define NULL_TERMINATOR_LENGTH 1
#define SLASH_LENGTH 1

typedef enum { MODE_PREPEND = 1, MODE_APPEND = 2 } Mode;

struct args {
  char dir_arg[PATH_MAX];
  char dir_path[PATH_MAX];
  char filtered_extension[NAME_MAX];
  char pattern[NAME_MAX];
  int mode;
};

int pattern_rename(struct args *current_args);

#endif
