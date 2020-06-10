#include "exec.h"

int exec(char** cmd)
{
    abs_path(cmd);
    int exit = 0;
    int st = 0;
    pid_t pid = 0;

    // fork
    pid = fork();
    if (pid == -1)
        perror("fork error");
    else if (pid > 0)
    {
        waitpid(pid, &st, 0);
        kill(pid, SIGTERM);
    }
    else
    {
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
        if (execve(cmd[0], cmd, NULL) == -1)
        {
            perror("Command not found");
            exit = 127;
        }
    }

    if (strcmp(cmd[0], "/usr/bin/cat") == 0 || strcmp(cmd[0], "/bin/cat") == 0
        || strcmp(cmd[0], "cat") == 0)
    {
        for (size_t i = 1; i < len_array(cmd); i++)
        {
            if (!fileExists(cmd[i]))
            {
                exit = 1;
            }
        }
    }

    return exit;
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
        parsed = parse(input, "&&");
    if (seq == 2)
        parsed = parse(input, "||");
    for (size_t i = 0; parsed[i]; i++)
    {
        int seq2 = is_sequence(parsed[i]);
        if (seq2 == 2)
        {
            exit = exec_sequence(parsed[i], seq2);
            continue;
        }
        char** cmd = parse(parsed[i], " \n\t");
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
            abs_path(cmd);
            if (cmd[0] == NULL)
            {
                printf("minishell: %s: command not found\n", tmp);
                exit = 127;
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
            else
            {
                exit = exec(cmd);
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
    char** parsed = parse(input, "<>>\n\t");
    int fd = -1;
    int fd_bis = -1;
    fd = fd;
    fd_bis = fd_bis;
    if (!parsed[1])
    {
        warnx("wrong use of redir");
        return 1;
    }
    strip_space(parsed[1]);
    switch (redir)
    {
    case 1:
        fd = open(parsed[1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        fd_bis = 1;
        break;
    case 2:
        fd = open(parsed[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        fd_bis = 1;
        break;
    case 3:
        fd = open(parsed[1], O_RDONLY);
        fd_bis = 0;
        break;
    }

    fflush(stdout);
    fflush(stderr);

    if (fd == -1)
    {
        warn("Redirection not handled");
        return 1;
    }

    int old = dup(fd_bis);
    dup2(fd, fd_bis);
    exec(parse(parsed[0], " \n\t"));
    close(fd);
    if (old != -1)
    {
        dup2(old, fd_bis);
        close(old);
    }
    return exit;
}
