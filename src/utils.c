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


void abs_path(char** cmd)
{
    char** parsed_path = NULL;
    char* path = strdup(getenv("PATH"));
    char* bin = NULL;

        if (cmd[0] != NULL && cmd[0][0] != '/' && strncmp(cmd[0], "./", 2) != 0)
        {
            parsed_path = parse(path, ":");
            free(path);
            path = NULL;

            for (size_t i = 0; parsed_path[i]; i++)
            {
                bin = calloc(sizeof(char), strlen(parsed_path[i]) + 1 + strlen(cmd[0]) + 1);
                if (bin == NULL)
                    break;

                strcat(bin, parsed_path[i]);
                strcat(bin, "/");
                strcat(bin, cmd[0]);

                if (!access(bin, F_OK))
                    break;

                free(bin);
                bin = NULL;
            }

            free_array(parsed_path);
            free(cmd[0]);
            cmd[0] = bin;
        }
        else
        {
            free(path);
            path = NULL;
        }
}

int is_redir(char* input)
{
    char prev = input[0];
    int exit = 0;
    for (size_t i = 0; input[i]; i++)
    {
        if (input[i] == '>' && prev != '>')
            return 1;
        else if (input[i] == '<' && prev != '<')
            exit = 2;
        else if (input[i] == '>' && prev == '>')
            exit = 3;
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
