#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"

void free_array(char** rray);
size_t len_array(char** array);
int is_sequence(char* input);
