#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>

#include "parse.h"
#include "utils.h"
#include "builtin.h"

int exec(char** cmd);
int exec_builtin(char** cmd);
int exec_sequence(char* input, int seq);
int exec_redir(char* input, int redir);
int exec_pipe(char* input);
