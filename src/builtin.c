#include "builtin.h"


bool is_builtin(char* cmd)
{
    const char* builtins[] = { "cd", "echo", "exit", "kill", NULL };
    for (size_t i = 0; builtins[i]; i++)
        if (!strcmp(cmd, builtins[i]))
            return true;
    return false;
}

void cd_home()
{
    const char* const home = getenv("HOME");
    if (home)
        if (chdir(home) == -1)
            perror("chdir()");
}

void my_cd(char* path)
{
    if (strcmp(path, "~") == 0)
        cd_home();
    else if (chdir(path) == -1)
        perror("chdir()");
}

void my_echo(char** cmd)
{
    int n = 0;
    int s = 0;
    size_t l = len_array(cmd);
    for (size_t i = 1; i < l && cmd[i]; i++)
    {
        size_t len = strlen(cmd[i]);
        if (!strcmp(cmd[i], "-n"))
        {
            n = 1;
            if (l > 3 && !s)
            {
                s = 1;
                memmove(cmd[2], cmd[2] + 1, strlen(cmd[1]));
            }
            continue;
        }
        else if (l > 2 && !s)
        {
            s = 1;
            memmove(cmd[1], cmd[1] + 1, strlen(cmd[1]));
        }
        if (s && i == l - 1)
        {
            cmd[i][len - 1] = 0;
        }
        // strip quotes at begin and end of string
        if (cmd[i][0] == '"' && cmd[i][len - 1] == '"')
        {
            if (len > 2)
            {
                memmove(cmd[i], cmd[i] + 1, len - 2);
                cmd[i][len - 2] = 0;
            }
        }
        printf("%s", cmd[i]);
        if (s && i < l - 1)
            printf(" ");
    }
    if (!n)
        printf("\n");
}

void my_exit(char** cmd)
{
    int ret = 0;
    if (len_array(cmd) > 1)
        ret = atoi(cmd[1]);
    free_array(cmd);
    cmd = NULL;
    exit(ret);
}

void my_kill(char** cmd)
{
    int signal = -atoi(cmd[1]);
    for (size_t i = 3; cmd[i]; i++)
    {
        int pid = atoi(cmd[i]);
        if (kill(pid, signal) != 0)
            printf("no such process");
    }
}
