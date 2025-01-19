#include "include/memory.h"

void *memset(void *ptr, int c, size_t n)
{
    char *char_ptr = (char *)ptr;
    for (int i = 0; i < n; i++)
    {
        char_ptr[i] = (char)c;
    }
    return ptr;
}
