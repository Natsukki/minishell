#define _GNU_SOURCE
#include <err.h>

#include "parse.h"
#include "exec.h"
#include "builtin.h"

void sigintHandler (int sig_num)
{
    sig_num = sig_num;
    write(1, "\nminishell$ ", 12);
    signal(SIGINT, sigintHandler);
}


void shell_loop(FILE *file)
{
    size_t input_size = 2048;
    char* input = (char *)calloc(sizeof(char), input_size);
    if (!input)
    {
        free(input);
        err(12, "calloc error");
    }
    //write prompt
    if (isatty (fileno(file)))
        write(1, "minishell$ ", 11);
    signal(SIGINT, sigintHandler);
    while (getline(&input, &input_size, file) > 0)
    {
        if (input[0] == '\0')
            continue;

        // handling empty input
        if (input[0] == '\n')
        {
            if (isatty (fileno(file)))
                write(1, "minishell$ ", 11);
            continue;
        }
        char** cmd = parse(input, " \n\t");
        if (cmd[0] == NULL)
            printf("Command not found\n");
        else if(is_builtin(cmd[0]) == false)
        {
            abs_path(cmd);
            exec(cmd);
        }
        else
            exec_builtin(cmd);
        if (isatty (fileno(file)))
            write(1, "minishell$ ", 11);
        free_array(cmd);
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
