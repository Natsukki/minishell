CC=gcc
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c99
SRC= src/main.c


all :
	${CC} ${CFLAGS} ${SRC} -o minishell

clean :
	rm minishell
