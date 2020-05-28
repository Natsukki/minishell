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
