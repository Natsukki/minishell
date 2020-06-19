#include "builtin.h"

int my_exit(char **cmd)
{
    int ret = 0;
    size_t l = len_array(cmd);
    if (l > 2)
    {
        warnx("exit: too many arguments");
        exit(1);
    }
    if (l > 1)
    {
        if (!is_numerical(cmd[1]))
        {
            warnx("exit: numeric argument required");
            exit(2);
        }
        ret = atoi(cmd[1]);
    }
    free_array(cmd);
    cmd = NULL;
    exit(ret);
    return ret;
}
