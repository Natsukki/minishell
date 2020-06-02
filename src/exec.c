#include "exec.h"

int exec(char** cmd)
{
    int exit = 0;
    int st = 0;
    pid_t pid = 0;

    // fork
    pid = fork();
    if (pid == -1)
        perror("fork error");
    else if (pid > 0)
    {
        waitpid(pid, &st, 0);
        kill(pid, SIGTERM);
    }
    else
    {

        // strip quotes from the string to echo
        if (strcmp(cmd[0], "/usr/bin/echo") == 0
            || strcmp(cmd[0], "/bin/echo") == 0)
        {
            for (size_t i = 0; i < len_array(cmd); i++)
            {
                size_t len = strlen(cmd[i]);
                strip_quotes(i, len, cmd);
            }
        }
        if (execve(cmd[0], cmd, NULL) == -1)
        {
            perror("Command not found");
            exit = 127;
        }
//        exit(EXIT_FAILURE);
    }
    return exit;
}

int exec_builtin(char** cmd)
{
    int exit = 0;
    if (!strcmp(cmd[0], "cd"))
        exit = my_cd(cmd[1]);
    if (!strcmp(cmd[0], "echo"))
        exit = my_echo(cmd);
    if (!strcmp(cmd[0], "exit"))
        exit = my_exit(cmd);
    if (!strcmp(cmd[0], "kill"))
        exit = my_kill(cmd);
    return exit;
}

int exec_sequence(char* input, int seq)
{
    int exit = 0;
    char** parsed;
    if (seq == 1)
        parsed = parse(input, "&&");
    if (seq == 2)
        parsed = parse(input, "||");
    for (size_t i = 0; parsed[i]; i++)
    {
        int seq2 = is_sequence(parsed[i]);
        if (seq2 == 2)
        {
            exit = exec_sequence(parsed[i], seq2);
            continue;
        }
        char** cmd = parse(parsed[i], " \n\t");
        if(is_builtin(cmd[0]) == false)
        {
            if (!strcmp(cmd[0], "false"))
                {
                    exit = 1;
                    if (seq == 1)
                        break;
                    continue;
                }
            char* tmp = strdup(cmd[0]);
            abs_path(cmd);
            if (cmd[0] == NULL)
            {
                printf("minishell: %s: command not found\n", tmp);
                exit = 127;
                if (seq == 1)
                {
                    free(tmp);
                    tmp = NULL;
                    free_array(cmd);
                    break;
                }
                else
                {
                    free(tmp);
                    tmp = NULL;
                    free_array(cmd);
                    continue;
                }
            }
            else
            {
                exit = exec(cmd);
            }
            free(tmp);
            tmp = NULL;
        }
        else
            exit = exec_builtin(cmd);
        free_array(cmd);
        if (!exit && seq == 2)
            break;
    }
    free_array(parsed);
    return exit;
}


int exec_redir(char** cmd, int redir)
{
    cmd = cmd;
    redir = redir;
    return 0;
}
