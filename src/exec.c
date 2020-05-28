#include "exec.h"

void exec(char** cmd)
{
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
            size_t len = strlen(cmd[1]);
            if (cmd[1][0] == '"' && cmd[1][len - 1] == '"')
            {
                if (len > 2)
                {
                    memmove(cmd[1], cmd[1] + 1, len - 2);
                    cmd[1][len - 2] = 0;
                }
            }
        }
        if (execve(cmd[0], cmd, NULL) == -1)
            perror("Command not found");
        exit(EXIT_FAILURE);
    }
}

void exec_builtin(char** cmd)
{
    if (!strcmp(cmd[0], "cd"))
        my_cd(cmd[1]);
    if (!strcmp(cmd[0], "echo"))
        my_echo(cmd);
    if (!strcmp(cmd[0], "exit"))
        my_exit(cmd);
    if (!strcmp(cmd[0], "kill"))
        my_kill(cmd);
    else
        return;
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
