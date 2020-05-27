#include "parse.h"

static char **parse(char* line, char* limit)
{
    size_t index = 0;
    char *tok = NULL;
    char **cmd = NULL;

    tok = strtok(line, limit);

    while (tok)
    {
        cmd = (char **)realloc(cmd, (index + 1) * sizeof(char *));
        cmd[index] = strdup(tok);
        tok = strtok(NULL, limit);
        index++;
    }

    // NULL at end of array to avoid undefined behaviour
    cmd = (char **)realloc(cmd, (index + 1) * sizeof(char *));
    cmd[index] = NULL;

    return cmd;
}

static void free_array(char** array)
{
    for (size_t i = 0; array[i]; i++)
    {
        free(array[i]);
        array[i] = NULL;
    }

    free(array);
    array = NULL;
}
