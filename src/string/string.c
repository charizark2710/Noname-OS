#include "../include/string.h"

size_t strlen(const char *c)
{
    size_t len = 0;
    while (c[len])
    {
        if (len >= MAX_STRING)
        {
            return len;
        }
        len++;
    }

    return len;
}