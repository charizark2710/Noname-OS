#ifndef EXT2_H
#define EXT2_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "kheap.h"
#include "string.h"

// Ex: /home/root/os.bin
// blocks = [home, root, os.bin]
// next     [1,    1,    0]
#define BLOCK_SIZE 4096
#define MAX_BLOCK 256 // 1MB -> 1024 * 1024 / 4096 = 256
#define MAX_ENTRIES 10

struct meta_data
{
    char *id;
    int owner;
    bool is_directory;
};

struct block
{
    int index;
    char *start;
    char *end;
    struct block *next;
};

struct inode
{
    struct block *block;
    struct meta_data *meta_data;
};

struct directory
{
    int total_entries;
    struct inode *inode;
    struct inode *inode_entries[MAX_ENTRIES];
};

struct directory_table
{
    int total_entries;
    struct directory *directory_entries[MAX_ENTRIES];
};

void init_ext2();
static uint32_t align_upper(uint32_t val);
struct inode *create_inode(char* id, void *data, size_t size, int owner, struct directory *directory);
struct directory *create_dir(char *id, int owner, struct directory *parent_table);
struct inode *get_inode(char *id, struct directory *directory);
struct directory *get_file_directory(char *id);

#endif