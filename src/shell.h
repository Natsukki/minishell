#define _GNU_SOURCE
#include <err.h>

#include "parse.h"
#include "exec.h"
#include "builtin.h"

int shell_loop(FILE *file);
