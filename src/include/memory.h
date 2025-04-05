#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *memset(void *ptr, int c, size_t n);
int memcmp(const void *s1, const void *s2, int count);
void memcpy(void *dest, const void *src, size_t n);

#endif