#include "utils.h"


void free_array(char** array)
{
    for (size_t i = 0; array[i]; i++)
    {
        free(array[i]);
        array[i] = NULL;
    }

    free(array);
    array = NULL;
}


size_t len_array(char** array)
{
    size_t c = 0;
    for (size_t i = 0; array[i]; i++)
        c++;
    return c;
}

int is_sequence(char* input)
{
    char prev = input[0];
    int exit = 0;
    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == prev && prev == '&')
            return 1;
        else if (input[i] == prev && prev == '|')
            exit = 2;
        prev = input[i];
    }
    return exit;
}

void strip_quotes(int i, size_t len, char** cmd)
{
    if (cmd[i][0] == '"' && cmd[i][len - 1] == '"')
        {
            if (len > 2)
            {
                memmove(cmd[i], cmd[i] + 1, len - 2);
                cmd[i][len - 2] = 0;
            }
        }
    if (cmd[i][0] == '"')
    {
        memmove(cmd[i], cmd[i] + 1, strlen(cmd[i]));
    }
    if (cmd[i][len - 1] == '"')
    {
        cmd[i][len - 1] = 0;
    }
}

void strip_space(char *line)
{
    size_t l = strlen(line);
    for (size_t i = 0; i < l; i++)
    {
        if (line[i] == ' ')
        {
            for (size_t j = i; j < l; j++)
            {
                line[j] = line[j + 1];
            }
            l--;
        }
    }
}

int fileExists(const char *filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else
        return 0;
}


int is_redir(char* input)
{
    char prev = input[0];
    int exit = 0;
    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == '>' && prev != '>' && input[i + 1] != '>')
            return 1;
        else if (input[i] == '<')
             return 3;
        else if (input[i] == '>' && prev == '>')
            return  2;
        prev = input[i];
    }
    return exit;
}

int is_numerical(char* input)
{
    for (int i = 0; input[i]; i++)
    {
        if (input[i] < '0' || input[i] > '9')
            return 0;
    }
    return 1;
}

int is_echo(char** cmd)
{
    if (!strcmp("/usr/bin/echo", cmd[0])
                || !strcmp("/bin/echo", cmd[0])
                || !strcmp("echo", cmd[0]))
        return 1;
    return 0;
}

int is_pipe(char* input)
{
    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == '|')
            return 1;
    }
    return 0;
}
