#include "parse.h"

static char* to_string(int ret_code)
{
    char* ret = "0";
    switch(ret_code)
    {
    case 1:
        ret = "1";
        break;
    case 127:
        ret = "127";
        break;
    case 128:
        ret = "128";
        break;
    case 2:
        ret = "2";
        break;
    case 126:
        ret = "126";
        break;
    case 130:
        ret = "130";
        break;
    }
    return ret;
}

char **parse(char* line, char* limit, int ret_code)
{
    size_t index = 0;
    char *tok = NULL;
    char **cmd = NULL;

    tok = strtok(line, limit);

    while (tok)
    {
        cmd = realloc(cmd, (index + 1) * sizeof(char *));
        if (!strcmp(tok, "$?"))
        {
            char *todo = to_string(ret_code);
            cmd[index] = strdup(todo);
        }
        else
            cmd[index] = strdup(tok);
        tok = strtok(NULL, limit);
        index++;
    }

    // NULL at end of array to avoid undefined behaviour
    cmd = realloc(cmd, (index + 1) * sizeof(char *));
    cmd[index] = NULL;

    return cmd;
}

char **parse_no_ret(char* line, char* limit)
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
