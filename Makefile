CC=gcc
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c99
SRC= src/exec/* src/builtin/* src/parse/* src/shell/* src/utils/* src/main.c


all :
	${CC} ${CFLAGS} ${SRC} -o minishell

debug :
	${CC} ${CFLAGS} ${SRC} -g -o minishell

clean :
	rm minishell
