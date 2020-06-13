#define _GNU_SOURCE
#include <err.h>

#include "parse.h"
#include "exec.h"
#include "builtin.h"

int exit_code = 0;

void sigintHandler (int sig_num)
{
    sig_num = sig_num;
    printf("\nminishell$ ");
    signal(SIGINT, sigintHandler);
}


int shell_loop(FILE *file)
{
    int exit = 0;
    size_t input_size = 2048;
    char* input = calloc(sizeof(char), input_size);
    if (!input)
    {
        free(input);
        err(12, "calloc error");
    }
    //write prompt
    if (isatty(fileno(file)))
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
            int seq = is_sequence(parsed[i]);
            if (seq)
            {
                exit = exec_sequence(parsed[i], seq);
                exit_code = exit;
                continue;
            }
            int redir = is_redir(parsed[i]);
            if (redir)
            {
                exit = exec_redir(parsed[i], redir);
                exit_code = exit;
                continue;
            }

            char** cmd = parse(parsed[i], " \n\t");
            if (!strcmp("/usr/bin/echo", cmd[0])
                || !strcmp("/bin/echo", cmd[0])
                || !strcmp("echo", cmd[0]))
            {
                if (!strcmp(cmd[1], "$?"))
                {
                    printf("%d\n", exit_code);
                    exit = 0;
                    exit_code = exit;
                    free_array(cmd);
                    break;
                }
            }
            if(is_builtin(cmd[0]) == false)
            {
                if (!strcmp(cmd[0], "false"))
                {
                    exit = 1;
                    exit_code = 1;
                    free_array(cmd);
                    continue;
                }
                char* tmp = strdup(cmd[0]);
                if (cmd[0] == NULL)
                {
                    printf("minishell: %s: command not found\n", tmp);
                    exit = 127;
                    exit_code = exit;
                }
                else if (!strcmp(cmd[0], "false"))
                {
                    exit = 1;
                    exit_code = exit;
                }
                else
                {
                    exit = exec(cmd);
                    exit_code = exit;
                }
                free(tmp);
                tmp = NULL;
            }
            else
            {
                exit = exec_builtin(cmd);
                exit_code = exit;
            }
            free_array(cmd);
        }
        free_array(parsed);
        if (isatty (fileno(file)))
                printf("minishell$ ");
    }

    free(input);
    exit_code = exit;
    return exit;
}


int main(int argc, char** argv)
{
    int exit = 1;
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
            exit = shell_loop(file);
            fclose(file);
        }
    }
    else
        exit = shell_loop(stdin);
    return exit;
}
