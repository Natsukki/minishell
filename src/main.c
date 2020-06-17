#define _GNU_SOURCE

#include "parse.h"
#include "exec.h"
#include "builtin.h"
#include "shell.h"

int main(int argc, char** argv)
{
    int ret = 1;
    if (argc != 1)
    {
        FILE *file = fopen(argv[1], "r");
        if (!file)
        {
            fprintf(stderr, "minishell: %s: no such file or directory\n", argv[1]);
            return 127;
        }
        else
        {
            ret = shell_loop(file);
            fclose(file);
        }
    }
    else
        ret = shell_loop(stdin);
    return ret;
}
