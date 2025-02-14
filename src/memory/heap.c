#include "../include/heap.h"

static int heap_validate_alignment(void *ptr)
{
    return ((unsigned int)ptr % HEAP_SIZE_TOTAL) == 0;
}

static int heap_validate_table(struct heap *heap, struct heap_table *table)
{
    void *table_end = (void *)(heap->start + HEAP_SIZE_TOTAL);
    size_t table_size = table_end - heap->start;
    size_t total_block = table_size / HEAP_BLOCK_SIZE;
    if (total_block != table->total)
    {
        return VALIDATE_TOTAL_BLOCKS_ERROR;
    }
}

void calculate_table_size(struct heap_table *table)
{
    size_t total = table->total * sizeof(HEAP_BLOCK_TABLE_ENTRY);
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, total);
}

int heap_create(struct heap *heap, struct heap_table *table)
{
    int res = heap_validate_alignment(heap->start);
    if (!res)
    {
        return res;
    }

    if (heap_validate_table(heap, table) < 0)
    {
        return VALIDATE_TOTAL_BLOCKS_ERROR;
    }

    memset(heap->start, 0, sizeof(struct heap));
    calculate_table_size(table);

    heap->table = *table;
    return res;
}
