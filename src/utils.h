#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "parse.h"

void free_array(char** rray);
size_t len_array(char** array);
int is_sequence(char* input);
void strip_quotes(int i, size_t len, char** cmd);
int fileExists(const char *filename);
void abs_path(char** cmd);
