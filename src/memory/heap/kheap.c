#include "../../include/kheap.h"
#include "../../include/kernel.h"

struct heap kheap;
struct heap_table kheap_table;

void kheap_init()
{
    kheap_table.entries = (HEAP_BLOCK_TABLE_ENTRY *)HEAP_ADDRESS;
    // init how many entries (blocks) we can have in the table
    kheap_table.total = HEAP_SIZE_TOTAL / HEAP_BLOCK_SIZE;
    kheap.start = (void *)HEAP_START;
    int res = heap_create(&kheap, &kheap_table);
    if (res < 0)
    {
        print("Failed to create heap\n");
    }
}

static uint32_t align_upper(uint32_t val)
{
    if (val % HEAP_BLOCK_SIZE == 0)
    {
        return val;
    }
    return HEAP_BLOCK_SIZE + (val - (val % HEAP_BLOCK_SIZE));
}

void mark_block_taken(struct heap *heap, int start_block, int needed_blocks)
{
    HEAP_BLOCK_TABLE_ENTRY entry_val = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    HEAP_BLOCK_TABLE_ENTRY next_entry_val = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    if (needed_blocks > 1)
    {
        entry_val |= HEAP_BLOCK_HAS_NEXT;
        next_entry_val |= HEAP_BLOCK_HAS_NEXT;

        // First block mark with HEAP_BLOCK_IS_FIRST
        heap->table.entries[start_block] = entry_val;
        // set the blocks as taken with has next except the last one
        memset(heap->table.entries + 1 + start_block, next_entry_val, needed_blocks - 2);

        // set the last block as taken
        heap->table.entries[needed_blocks - 1 + start_block] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
    }
    else
    {
        heap->table.entries[start_block] = entry_val;
    }
}

void mark_block_free(struct heap *heap, int start_block)
{
    size_t size = 0;
    int temp = start_block;
    HEAP_BLOCK_TABLE_ENTRY *entry = heap->table.entries;
    do
    {
        size += HEAP_BLOCK_SIZE;
        entry[start_block] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        start_block++;
    } while ((entry[start_block] & 0x0f) != HEAP_BLOCK_TABLE_ENTRY_FREE && (entry[start_block] & 0x40) != HEAP_BLOCK_IS_FIRST);
    size_t saddr = (size_t)kheap.start + (temp * HEAP_BLOCK_SIZE);
    memset((void *)saddr, 0, size);
}

void *kmalloc(size_t size)
{
    uint32_t aligned_size = align_upper(size);
    if (aligned_size > HEAP_SIZE_TOTAL)
    {
        return 0;
    }
    uint32_t total_needed_blocks = aligned_size / HEAP_BLOCK_SIZE;
    int total_table_entries = kheap.table.total;
    int free_blocks = 0;
    int start_block = -1;
    for (size_t i = 0; i < total_table_entries; i++)
    {
        if ((kheap.table.entries[i] & 0x0f) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            free_blocks = 0;
            start_block = -1;
            continue;
        }
        if (start_block < 0)
        {
            start_block = i;
        }
        free_blocks++;
        if (free_blocks == total_needed_blocks)
        {
            mark_block_taken(&kheap, start_block, total_needed_blocks);
            size_t saddr = (size_t)kheap.start + (start_block * HEAP_BLOCK_SIZE);
            return (void *)saddr;
        }
    }
    if (start_block < 0)
    {
        print("Failed to create heap\n");
    }
    return 0;
}

void kfree(void *ptr)
{
    if (ptr == 0)
    {
        return;
    }
    int start_block = ((int)(ptr - kheap.start)) / HEAP_BLOCK_SIZE;
    mark_block_free(&kheap, start_block);
    return;
}