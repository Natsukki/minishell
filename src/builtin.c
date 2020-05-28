#include "builtin.h"


bool is_builtin(char* cmd)
{
    const char* builtins[] = { "cd", "echo", "exit", NULL};
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
    size_t l = len_array(cmd);
    for (size_t i = 1; i < l && cmd[i]; i++)
    {
        if (!strcmp(cmd[i], "-n"))
        {
            n = 1;
            continue;
        }
        // strip quotes at begin and end of string
        size_t len = strlen(cmd[i]);
        if (cmd[i][0] == '"' && cmd[i][len - 1] == '"')
        {
            if (len > 2)
            {
                memmove(cmd[i], cmd[i] + 1, len - 2);
                cmd[i][len - 2] = 0;
            }
        }
        printf("%s", cmd[i]);
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
