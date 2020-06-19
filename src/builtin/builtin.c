#include "builtin.h"

bool is_builtin(char *cmd)
{
    const char* builtins[] = { "cd", "echo", "exit", "kill", NULL };
    for (size_t i = 0; builtins[i]; i++)
        if (!strcmp(cmd, builtins[i]))
            return true;
    return false;
}
