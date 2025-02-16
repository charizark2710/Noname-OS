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