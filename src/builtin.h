#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>

#include "utils.h"

bool is_builtin(char* cmd);
int my_cd(char* path);
int my_echo(char** cmd);
int my_exit(char** cmd);
int my_kill(char** cmd);
