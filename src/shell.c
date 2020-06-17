#include "shell.h"


int ret_code = 0;


static void sigintHandler (int sig_num)
{
    sig_num = sig_num;
    printf("\nminishell$ ");
    signal(SIGINT, sigintHandler);
}

static char* to_string()
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

static void prompt(FILE *file)
{
    if (isatty(fileno(file)))
        printf("minishell$ ");
}


int shell_loop(FILE *file)
{
    int ret = 0;
    size_t input_size = 2048;
    char* input = calloc(sizeof(char), input_size);
    if (!input)
    {
        free(input);
        err(12, "calloc error");
    }
    prompt(file);
    signal(SIGINT, sigintHandler);
    while (getline(&input, &input_size, file) > 0)
    {
        // handling empty input
        if (input[0] == '\n' || input[0] == '\0')
        {
            prompt(file);
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
                ret = exec_sequence(parsed[i], seq);
                ret_code = ret;
                continue;
            }
            int redir = is_redir(parsed[i]);
            if (redir)
            {
                ret = exec_redir(parsed[i], redir);
                ret_code = ret;
                continue;
            }

            char** cmd = parse(parsed[i], " \n\t");
            if (!strcmp("/usr/bin/echo", cmd[0])
                || !strcmp("/bin/echo", cmd[0])
                || !strcmp("echo", cmd[0]))
            {
                if (!strcmp(cmd[1], "$?"))
                {
                    char* todo = to_string();
                    strcpy(cmd[1], todo);
                    ret = 0;
                    ret_code = ret;
                }
            }
            if(is_builtin(cmd[0]) == false)
            {
                if (!strcmp(cmd[0], "false"))
                {
                    ret = 1;
                    ret_code = 1;
                    free_array(cmd);
                    continue;
                }
                char* tmp = strdup(cmd[0]);
                if (cmd[0] == NULL)
                {
                    fprintf(stderr, "minishell: %s: command not found\n", tmp);
                    ret = 127;
                    ret_code = ret;
                }
                else if (!strcmp(cmd[0], "false"))
                {
                    ret = 1;
                    ret_code = ret;
                }
                else
                {
                    ret = exec(cmd);
                    ret_code = ret;
                }
                free(tmp);
                tmp = NULL;
            }
            else
            {
                ret = exec_builtin(cmd);
                ret_code = ret;
            }
            free_array(cmd);
        }
        free_array(parsed);
        prompt(file);
    }

    free(input);
    ret_code = ret;
    exit(ret);
    return ret;
}
