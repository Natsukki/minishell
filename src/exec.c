#include "exec.h"

static int my_wait(int pid)
{
    int ret;
    if (waitpid(pid, &ret, 0) == -1)
    {
        if (errno == EINTR)
            return 130;
        warn("error while waiting for child");
        return 1;
    }
    return WEXITSTATUS(ret);
}

int exec(char** cmd)
{
    pid_t pid = 0;

    // strip quotes from the string to echo
    if (strcmp(cmd[0], "/usr/bin/echo") == 0
        || strcmp(cmd[0], "/bin/echo") == 0)
    {
        for (size_t i = 0; i < len_array(cmd); i++)
        {
            size_t len = strlen(cmd[i]);
            strip_quotes(i, len, cmd);
        }
    }

    // fork
    pid = fork();
    if (pid)
        return my_wait(pid);
    else
    {
        execvp(cmd[0], cmd);
        int ret = 127;
        if (errno == ENOEXEC)
            ret = 126;
        errx(ret, "%s: command not found", cmd[0]);
    }
}

int exec_builtin(char** cmd)
{
    int exit = 0;
    if (!strcmp(cmd[0], "cd"))
    {
        if (len_array(cmd) > 2)
        {
            warnx("cd: too many arguments");
            return 1;
        }
        exit = my_cd(cmd[1]);
    }
    if (!strcmp(cmd[0], "echo"))
        exit = my_echo(cmd);
    if (!strcmp(cmd[0], "exit"))
        exit = my_exit(cmd);
    if (!strcmp(cmd[0], "kill"))
        exit = my_kill(cmd);
    return exit;
}

int exec_sequence(char* input, int seq)
{
    int exit = 0;
    char** parsed;
    if (seq == 1)
        parsed = parse_no_ret(input, "&&");
    if (seq == 2)
        parsed = parse_no_ret(input, "||");
    for (size_t i = 0; parsed[i]; i++)
    {
        int seq2 = is_sequence(parsed[i]);
        if (seq2 == 2)
        {
            exit = exec_sequence(parsed[i], seq2);
            continue;
        }
        char** cmd = parse_no_ret(parsed[i], " \n\t");
        if(is_builtin(cmd[0]) == false)
        {
            if (!strcmp(cmd[0], "false"))
            {
                exit = 1;
                if (seq == 1)
                    break;
                continue;
            }
            char* tmp = strdup(cmd[0]);
            exit = exec(cmd);
            if (exit == 127)
            {
                if (seq == 1)
                {
                    free(tmp);
                    tmp = NULL;
                    free_array(cmd);
                    break;
                }
                else
                {
                    free(tmp);
                    tmp = NULL;
                    free_array(cmd);
                    continue;
                }
            }
            free(tmp);
            tmp = NULL;
        }
        else
            exit = exec_builtin(cmd);
        free_array(cmd);
        if (!exit && seq == 2)
            break;
    }
    free_array(parsed);
    return exit;
}


int exec_redir(char* input, int redir)
{
    int exit = 0;
    char** parsed = parse_no_ret(input, "<>>\n\t");
    int fd = -1;
    int fd_bis = -1;
    if (!parsed[1])
    {
        warnx("wrong use of redir");
        free_array(parsed);
        return 1;
    }
    for (size_t i = 1; parsed[i]; i++)
    {
        strip_space(parsed[i]);
        switch (redir)
        {
        case 1:
            fd = open(parsed[i], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            fd_bis = 1;
            break;
        case 2:
            fd = open(parsed[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
            fd_bis = 1;
            break;
        case 3:
            fd = open(parsed[i], O_RDONLY);
            fd_bis = 0;
            break;
        }
    }

    fflush(stdout);
    fflush(stderr);

    if (fd == -1)
    {
        warn("Redirection not handled");
        free_array(parsed);
        return 1;
    }

    int old = dup(fd_bis);
    dup2(fd, fd_bis);

    char** cmd = parse_no_ret(parsed[0], " \n\t");
    if (is_builtin(cmd[0]))
        exit = exec_builtin(cmd);
    else
        exit = exec(cmd);
    free_array(cmd);
    close(fd);
    if (old != -1)
    {
        dup2(old, fd_bis);
        close(old);
    }

    free_array(parsed);
    return exit;
}

int exec_pipe(char** cmd)
{
    cmd = cmd;
    return 1;
}
