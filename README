# Minishell

As beautiful as a shell


## About

Minishell is a minimum viable version of a real shell.
The main goal is to have a good understanding of process creation and
synchronisation using the C programmming language.

## Installation & Usage

### Requirements
The only requirements are:
- GNU make (v3.81)
- GCC (v4.2.1)

Those versions are the ones used during development.

### Building the program

1. Download/Clone this repo

        git clone https://github.com/Natsukki/minishell
2. `cd` into the root directory and run `make`

        cd minishell
        make

### Running the program

After building the source, run `./minishell` from the project root.

Alternatively you can run `echo "echo Test." | ./minishell` to pipe commands in.

## Main Project Instructions

### The project

- Has no memory leaks
- Can run in interactive mode or get inputs directly from a pipe
- Is able to execute basic commands and commands with arguments
- Implemented a series of builtins: `echo`, `cd`, `exit`, `kill`
- Manages the errors by displaying an adapted message
- Handles simple command sequences: `&&`, `;`, `||`
```
minishell$ true && echo OK;
OK
minishell$ false || echo OK
OK
```
- Handles redirections: simple redirections (stdout and stderr), double redirections (stdout and stderr) and simple left redirections (stdin):
```
minishell$ echo login_x > AUTHORS
minishell$ cat AUTHORS
login_x
minishell$ cat /etc/passwd >> users_list
minishell$ grep ^root users_list
root:x:0:0:root:/root:/bin/bash
rootme:x:812:800:rootme lse:/home/rootme/:/bin/tcsh
minishell$ < users_list wc -l
9364
```
- Handles pipes: `|`:
```
minishell$ cat /etc/passwd | head -1
root:x:0:0:root:/root:/bin/zsh
```
- Handles return codes:
```
minishell$ true
minishell$ /bin/echo $?
0
```
- Only uses standard C library functions

### Testing

- run the `./check.sh` script (default testsuite options)
- Alternatively run the `testsuite.py` script:
```
Usage :  ./testsuite.py [-s] [-l] [-c] [-t] minishell

optional arguments:
  -s, --sanity     runs the tests with valgrind activated to check for leaks
  -l, --list       runs the tests and displays their categories
  -c, --category   runs only the tests of the given category
  -t, --timeout    runs tests with a given timeout option
```

Enjoy!
