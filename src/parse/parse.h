#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * parses an input line
 * @param input the inputed line
 * @param limit the delimiter
 * @param ret_code global return code
 */
char **parse(char* line, char* limit, int ret_code);

/**
 * parses an input line
 * @param input the inputed line
 * @param limit the delimiter
 */
char **parse_no_ret(char* line, char* limit);
