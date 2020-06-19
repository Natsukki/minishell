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

static void flush()
{
    fflush(stdout);
    fflush(stderr);
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

static int in_redir_one(char** parsed, int fd, int fd_bis, int exit)
{
    char* tmp = parsed[0];
    char** cmd = parse_no_ret(tmp, " \n\t");
    fd = open(cmd[0], O_RDONLY);
    fd_bis = 0;
    char** bis = NULL;
    size_t i = 0;
    for (; cmd[i + 1]; i++)
    {
        bis = realloc(bis, (i + 1) * sizeof(char *));
        bis[i] = strdup(cmd[i + 1]);
    }
    bis = realloc(bis, (i + 1) * sizeof(char *));
    bis[i] = NULL;

    flush();
    if (fd == -1)
    {
        warn("Redirection not handled");
        free_array(parsed);
        return 1;
    }

    int old = dup(fd_bis);
    dup2(fd, fd_bis);

    exit = exec(bis);
    free_array(cmd);
    free_array(bis);
    close(fd);
    if (old != -1)
    {
        dup2(old, fd_bis);
        close(old);
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
    size_t l = len_array(parsed);
    if (l == 1 && redir == 3)
    {
        return in_redir_one(parsed, fd, fd_bis, exit);
    }
    else
    {
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

        flush();

        if (fd == -1)
        {
            warn("Redirection not handled");
            free_array(parsed);
            return 1;
        }

        int old = dup(fd_bis);
        dup2(fd, fd_bis);

        char** cmd = parse_no_ret(parsed[0], " \n\t");
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
}

static int loop_pipe(char ***cmd)
{
  int   p[2];
  pid_t pid;
  int   fd_in = 0;
  int ret = 0;

  while (*cmd != NULL)
  {
      pipe(p);
      if ((pid = fork()) == -1)
      {
          exit(EXIT_FAILURE);
      }
      else if (pid == 0)
      {
          dup2(fd_in, 0); //change the input according to the old one
          if (*(cmd + 1) != NULL)
              dup2(p[1], 1);
          close(p[0]);
          execvp((*cmd)[0], *cmd);
          exit(EXIT_FAILURE);
      }
      else
      {
          ret = my_wait(pid);
          close(p[1]);
          fd_in = p[0]; //save the input for the next command
          cmd++;
      }
  }
  return ret;
}

static void free_triple_array(char*** array)
{
    for (size_t i = 0; array[i]; i++)
    {
        for (size_t j = 0; array[i][j]; j++)
        {
            free(array[i][j]);
            array[i][j] = NULL;
        }
        free(array[i]);
        array[i] = NULL;
    }
    free(array);
    array = NULL;
}

int exec_pipe(char* input)
{
    int ret = 0;
    char** cmd = parse_no_ret(input, "|");
    char*** tab = NULL;
    size_t i = 0;
    for (; cmd[i]; i++)
    {
        tab = realloc(tab, (i+ 1) * sizeof(char **));
        char** tmp = parse_no_ret(cmd[i], " \n\t");
        tab[i] = tmp;
    }
    tab = realloc(tab, (i + 1)* sizeof(char **));
    tab[i] = NULL;
    ret = loop_pipe(tab);
    free_array(cmd);
    free_triple_array(tab);
    return ret;
}
