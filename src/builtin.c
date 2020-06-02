#include "builtin.h"


bool is_builtin(char* cmd)
{
    const char* builtins[] = { "cd", "echo", "exit", "kill", NULL };
    for (size_t i = 0; builtins[i]; i++)
        if (!strcmp(cmd, builtins[i]))
            return true;
    return false;
}

int cd_home()
{
    int exit = 0;
    const char* const home = getenv("HOME");
    if (home)
        if (chdir(home) == -1)
        {
            perror("chdir()");
            exit = 1;
        }
    return exit;
}

int my_cd(char* path)
{
    int exit = 0;
    if (strcmp(path, "~") == 0)
        return cd_home();
    else if (chdir(path) == -1)
    {
        perror("chdir()");
        exit = 1;
    }
    return exit;
}

int my_echo(char** cmd)
{
    int exit = 0;
    int n = 0;
    size_t l = len_array(cmd);
    for (size_t i = 1; i < l && cmd[i]; i++)
    {
        size_t len = strlen(cmd[i]);
        if (!strcmp(cmd[i], "-n"))
        {
            n = 1;
            continue;
        }
        // strip quotes at begin and end of string
        strip_quotes(i, len, cmd);
        printf("%s", cmd[i]);
        if (l > 2 && !n)
            printf(" ");
    }
    if (!n)
        printf("\n");
    return exit;
}

int my_exit(char** cmd)
{
    int ret = 0;
    if (len_array(cmd) > 1)
        ret = atoi(cmd[1]);
    free_array(cmd);
    cmd = NULL;
    exit(ret);
    return ret;
}

int my_kill(char** cmd)
{
    int exit = 0;
    int signal = -atoi(cmd[1]);
    for (size_t i = 3; cmd[i]; i++)
    {
        int pid = atoi(cmd[i]);
        if (kill(pid, signal) != 0)
        {
            printf("no such process");
            exit = 1;
        }
    }
    return exit;
}
