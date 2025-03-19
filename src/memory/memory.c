#include "../include/memory.h"

void *memset(void *ptr, int c, size_t n)
{
    char *char_ptr = (char *)ptr;
    for (int i = 0; i < n; i++)
    {
        char_ptr[i] = (char)c;
    }
    return ptr;
}

int memcmp(const void *s1, const void *s2, int count)
{
    char *c1 = (char *)s1;
    char *c2 = (char *)s2;

    for (int i = 0; i < count; i++)
    {
        if (c1[i] != c2[i])
        {
            return c1[i] < c2[i] ? -1 : 1;
        }
    }

    return 0; // s1 and s2 are equal
}
