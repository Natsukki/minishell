#define _GNU_SOURCE
#include <err.h>

#include "../parse/parse.h"
#include "../exec/exec.h"
#include "../builtin/builtin.h"

/**
 * runs through the shell
 * @param file stream of the opened input source
 */
int shell_loop(FILE *file);
