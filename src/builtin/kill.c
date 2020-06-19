#include "builtin.h"

int my_kill(char **cmd)
{
    int signal = 15;
    int exit = 0;
    size_t l = len_array(cmd);
    if (l == 1)
    {
        fprintf(stderr, "kill: usage: kill [-s sigspec | -n signum | -sigspec]\
 pid | jobspec ... or kill -l [sigspec]\n");
        return 2;
    }
    if (!is_numerical(cmd[1]))
    {
        warnx("kill: %s: arguments must be process or job IDs", cmd[1]);
        exit = 1;
    }
    signal = -atoi(cmd[1]);
    for (size_t i = 2; cmd[i]; i++)
    {
        if (!is_numerical(cmd[i]))
            {
                warnx("kill: %s: arguments must be process or job IDs", cmd[i]);
                exit = 1;
                continue;
            }
        int pid = atoi(cmd[i]);
        if (kill(pid, signal) != 0)
        {
            warnx("kill: (%d) - No such process", pid);
            exit = 1;
        }
    }
    return exit;
}
