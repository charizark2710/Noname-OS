#ifndef PAGE_H

#define PAGE_H

#include <stdint.h>
#include "kheap.h"
#include <stdbool.h>

#define PAGE_SIZE 4096
#define PAGE_TOTAL_ENTRIES_PER_TABLE 1024

#define PAGE_CACHE_DISABLED 0b00010000
#define PAGE_WRITE_THROUGH 0b00001000
#define PAGE_USER_MODE 0b00000100
#define PAGE_WRITABLE 0b00000010
#define PAGE_PRESENT 0b00000001

struct page_4gb_chunk
{
    uint32_t *directory;
};

void switch_page(uint32_t *page);
struct page_4gb_chunk *new_4gb_page(uint8_t flag);
uint32_t *get_directory(struct page_4gb_chunk *chunk);
int *get_phys_idx(void *virtual_address);
// val is physical address along with flags
uint32_t memory_map(uint32_t *physical_addr, void *virtual_address, uint32_t val);

extern void load_page_directory(uint32_t *directory);
extern void enable_paging();

#endif