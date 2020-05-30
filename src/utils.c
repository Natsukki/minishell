#include "utils.h"


void free_array(char** array)
{
    for (size_t i = 0; array[i]; i++)
    {
        free(array[i]);
        array[i] = NULL;
    }

    free(array);
    array = NULL;
}


size_t len_array(char** array)
{
    size_t c = 0;
    for (size_t i = 0; array[i]; i++)
        c++;
    return c;
}

int is_sequence(char* input)
{
    char** and = parse(input, "&&");
    char** or = parse(input, "||");
    int ret = 0;
    if (len_array(and) > 1)
        ret = 1;
    else if (len_array(or) > 1)
        ret = 2;
    return ret;
}
