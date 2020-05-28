#define _GNU_SOURCE
#include <err.h>

#include "parse.h"
#include "exec.h"
#include "builtin.h"

void sigintHandler (int sig_num)
{
    sig_num = sig_num;
    printf("\nminishell$ ");
    signal(SIGINT, sigintHandler);
}


void shell_loop(FILE *file)
{
    size_t input_size = 2048;
    char* input = calloc(sizeof(char), input_size);
    if (!input)
    {
        free(input);
        err(12, "calloc error");
    }
    //write prompt
    if (isatty (fileno(file)))
        printf("minishell$ ");
    signal(SIGINT, sigintHandler);
    while (getline(&input, &input_size, file) > 0)
    {
        // handling empty input
        if (input[0] == '\n' || input[0] == '\0')
        {
            if (isatty (fileno(file)))
                printf("minishell$ ");
            continue;
        }
        char** parsed = parse(input, ";");
        for (size_t i = 0; parsed[i]; i++)
        {
            if (!strcmp(parsed[i], "\n"))
                break;
            char** cmd = parse(parsed[i], " \n\t");
            if(is_builtin(cmd[0]) == false)
            {
                char* tmp = strdup(cmd[0]);
                abs_path(cmd);
                if (cmd[0] == NULL)
                {
                    printf("minishell: %s: command not found\n", tmp);
                    free(tmp);
                    tmp = NULL;
                }
                exec(cmd);
            }
            else
                exec_builtin(cmd);
            free_array(cmd);
        }
        free_array(parsed);
            if (isatty (fileno(file)))
                printf("minishell$ ");
    }

    free(input);
}


int main(int argc, char** argv)
{
    if (argc != 1)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            fprintf(stderr, "minishell: %s: no such file or directory\n", argv[1]);
            return 127;
        }
        else
        {
            shell_loop(file);
            fclose(file);
        }
    }
    else
        shell_loop(stdin);
}
