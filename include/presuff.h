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

static const char dot[] = ".";

int pattern_rename(const char *directoryPath, const char *pattern,
                   const char *filteredExtension, long mode);
#endif