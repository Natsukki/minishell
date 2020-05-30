#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

#include "parse.h"
#include "utils.h"
#include "builtin.h"

int exec(char** cmd);
int exec_builtin(char** cmd);
void abs_path(char** cmd);
