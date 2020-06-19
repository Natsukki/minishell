#include "builtin.h"

int my_echo(char **cmd)
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
