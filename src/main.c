#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int main()
{
    size_t input_size = 2048;
    char* input = (char *)calloc(sizeof(char), input_size);
    if (!input)
    {
        free(input);
        err(12, "calloc error");
    }
    //write prompt
    write(1, "$>", 3);
    while (getline(&input, &input_size, stdin) > 0)
    {
        printf("prout %s\n", input);
        write(1, "$>", 3);
    }

    free(input);
}
