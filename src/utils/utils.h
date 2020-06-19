#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "../parse/parse.h"

/**
 * frees an char** array
 */
void free_array(char** rray);

/**
 * computes the length of an char** array
 */
size_t len_array(char** array);

/**
 * checks if input is a sequence
 */
int is_sequence(char* input);

/**
 * strips quotes
 * @param i index from which to start
 * @param len length of array
 * @param cmd array to strip
 */
void strip_quotes(int i, size_t len, char** cmd);

/**
 * checks if file exists
 */
int fileExists(const char *filename);

/**
 * checks if input is a redir
 */
int is_redir(char* input);

/**
 * checks if input is numerical
 */
int is_numerical(char* input);

/**
 * strips all spaces from char*
 */
void strip_space(char* line);

/**
 * checks if cmd is a call to echo
 */
int is_echo(char** cmd);

/**
 * chekcs if input is a pipe
 */
int is_pipe(char* input);
