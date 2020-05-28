#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"

bool is_builtin(char* cmd);
void my_cd(char* path);
void my_echo(char** cmd);
void my_exit(char** cmd);
void my_kill(char** cmd);
