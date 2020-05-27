#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static void free_array(char **array);
static char **parse(char* line, char* limit);
