CC=gcc
CFLAGS= -Wall -Wextra -Werror -pedantic -std=c99
SRC= src/*


all :
	${CC} ${CFLAGS} ${SRC} -o minishell

debug :
	${CC} ${CFLAGS} ${SRC} -g -o minishell

clean :
	rm minishell
