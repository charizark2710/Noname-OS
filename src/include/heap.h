#ifndef HEAP_H

#define HEAP_H

#include <stdint.h>
#include <stddef.h>

#include "../config.h"
#include "status.h"
#include "memory.h"


#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000
#define HEAP_BLOCK_IS_LAST 0b11000000
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;
struct heap_table
{
    HEAP_BLOCK_TABLE_ENTRY *entries;
    size_t total;
};

struct heap
{
    struct heap_table table;
    void *start;
};

int heap_create(struct heap *heap, struct heap_table *table);

#endif