#include "../../include/page.h"

static uint32_t *current_directory = 0;

struct page_4gb_chunk *new_4gb_page(uint8_t flag)
{
    uint32_t *dir = kmalloc(PAGE_TOTAL_ENTRIES_PER_TABLE * sizeof(uint32_t));
    int dir_index = 0;
    for (int i = 0; i < PAGE_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t *entries = kmalloc(PAGE_TOTAL_ENTRIES_PER_TABLE * sizeof(uint32_t));
        for (int j = 0; j < PAGE_TOTAL_ENTRIES_PER_TABLE; j++)
        {
            entries[j] = ((j * PAGE_SIZE) + dir_index) | flag;
        }
        dir_index += (PAGE_SIZE * PAGE_TOTAL_ENTRIES_PER_TABLE);
        dir[i] = (size_t)entries | flag;
    }

    struct page_4gb_chunk *chunk = kmalloc(sizeof(struct page_4gb_chunk));
    chunk->directory = dir;
    return chunk;
}

uint32_t *get_directory(struct page_4gb_chunk *chunk)
{
    return chunk->directory;
}

void switch_page(uint32_t *directory)
{
    load_page_directory(directory);
    current_directory = directory;
}

bool isAligned(void *addr)
{
    return (size_t)addr % PAGE_SIZE == 0;
}

int *get_phys_idx(void *virtual_address)
{
    if (!isAligned(virtual_address))
    {
        return NULL;
    }

    uint32_t directory_index = (size_t)virtual_address >> 22;                                    // PAGE_TOTAL_ENTRIES_PER_TABLE * PAGE_SIZE is 2^22
    uint32_t table_index = ((size_t)virtual_address >> 12) & (PAGE_TOTAL_ENTRIES_PER_TABLE - 1); //((size_t)virtual_address / PAGE_SIZE) % PAGE_TOTAL_ENTRIES_PER_TABLE;

    return (int[]){directory_index, table_index};
}

uint32_t memory_map(uint32_t *physical_addr, void *virtual_address, uint32_t val)
{
    int *indexes = get_phys_idx(virtual_address);
    if (indexes == NULL)
    {
        return -1;
    }

    uint32_t directory_index = indexes[0];
    uint32_t entry_index = indexes[1];

    size_t directory = physical_addr[directory_index];
    size_t *table = (size_t *)(directory & 0xfffff000);
    table[entry_index] = val;
}
