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
}
