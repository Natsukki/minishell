#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char **parse(char* line, char* limit, int ret_code);
char **parse_no_ret(char* line, char* limit);
