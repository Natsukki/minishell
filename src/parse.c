#include "parse.h"

char **parse(char* line, char* limit)
{
    size_t index = 0;
    char *tok = NULL;
    char **cmd = NULL;

    tok = strtok(line, limit);

    while (tok)
    {
        cmd = realloc(cmd, (index + 1) * sizeof(char *));
        cmd[index] = strdup(tok);
        tok = strtok(NULL, limit);
        index++;
    }

    // NULL at end of array to avoid undefined behaviour
    cmd = realloc(cmd, (index + 1) * sizeof(char *));
    cmd[index] = NULL;

    return cmd;
}
