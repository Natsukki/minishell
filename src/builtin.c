#include "builtin.h"


bool is_builtin(char* cmd)
{
    const char* builtins[] = { "cd", NULL};
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
