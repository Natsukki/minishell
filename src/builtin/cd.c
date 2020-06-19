#include "builtin.h"

int cd_home()
{
    int exit = 0;
    const char* const home = getenv("HOME");
    if (home)
        if (chdir(home) == -1)
        {
            warn("cd: %s", home);
            exit = 1;
        }
    return exit;
}

int my_cd(char *path)
{
    int exit = 0;
    if (strcmp(path, "~") == 0)
        return cd_home();
    else if (chdir(path) == -1)
    {
        warn("cd: %s", path);
        exit = 1;
    }
    return exit;
}
