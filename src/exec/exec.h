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

#include "../parse/parse.h"
#include "../utils/utils.h"
#include "../builtin/builtin.h"
/**
 * exec a basic command array
 * @param cmd the cmd array
*/
int exec(char** cmd);

/**
 * execute a builtin command
 * @param cmd the cmd array
*/
int exec_builtin(char** cmd);

/**
 * execute a sequence of instructions
 * @param input the inputed line
 * @param seq the ID of the sequence
 */
int exec_sequence(char* input, int seq);

/**
 * execute a sequence of redirections
 * @param input the input line
 * @param redir the ID of the redirection
*/
int exec_redir(char* input, int redir);

/**
 * execute a sequence of pipes
 * @param input the input line
*/
int exec_pipe(char* input);
