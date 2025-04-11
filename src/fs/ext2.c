#include "../include/ext2.h"

struct directory root_directory;
struct directory_table directory_table;

void init_ext2()
{
    struct inode *root_inode = kmalloc(sizeof(struct inode));
    root_inode->meta_data->id = "root";
    root_inode->block = kmalloc(BLOCK_SIZE);
    root_inode->block = 0x0;
    root_directory.inode = root_inode;
    root_directory.total_entries = 0;

    directory_table.total_entries = 0;
    directory_table.directory_entries[directory_table.total_entries++] = &root_directory;
}


static uint32_t align_upper(uint32_t val)
{
    uint32_t blocks = val / BLOCK_SIZE;
    if (val % BLOCK_SIZE == 0)
    {
        return blocks;
    }
    return ++blocks;
}

struct inode *create_inode(char *id, void *data, size_t size, int owner, struct directory *directory)
{
    if (directory == NULL)
    {
        directory = &root_directory;
    }
    size_t needed_blocks = align_upper(size);

    if (needed_blocks > MAX_BLOCK)
    {
        return 0x0;
    }
    struct inode *inode = kmalloc(sizeof(struct inode));
    inode->meta_data = kmalloc(sizeof(struct meta_data));
    inode->meta_data->owner = owner;

    size_t c_block_data = (size_t)data;
    size_t c_block_addr;
    for (int block_idx = 0; block_idx < needed_blocks; block_idx++)
    {
        c_block_data += (block_idx * BLOCK_SIZE);
        if (block_idx == 0)
        {
            inode->block = kmalloc(sizeof(struct block));
            inode->block->start = (void *)c_block_data;
            inode->block->end = (void *)c_block_data + BLOCK_SIZE - 1;
            c_block_addr = (size_t)inode->block;
        }
        else
        {
            struct block *current_block = (struct block *)c_block_addr;
            struct block *next_block = kmalloc(sizeof(struct block));
            current_block->next = next_block;
            next_block->start = (void *)c_block_data;
            next_block->end = (void *)c_block_data + BLOCK_SIZE;
            c_block_addr = (size_t)next_block;
        }
        if (block_idx == needed_blocks - 1)
        {
            size_t remain = (size_t)((struct block *)c_block_addr)->end - ((size_t)data + size - 1);
            if (remain > 0)
            {
                // padding the remain bytes
                memset((void *)c_block_data, 0x1, remain);
            }
        }
    }

    inode->meta_data->id = id;
    directory->inode_entries[directory->total_entries++] = inode;
    return inode;
}

struct directory *create_dir(char *id, int owner, struct directory *parent_directory)
{
    if (parent_directory == NULL)
    {
        parent_directory = &root_directory;
    }

    struct inode *inode = create_inode(id, 0x0, 0, owner, parent_directory);
    inode->meta_data->is_directory = true;
    struct directory *child_directory = kmalloc(sizeof(struct directory));
    child_directory->inode = inode;
    child_directory->total_entries = 0;

    directory_table.directory_entries[directory_table.total_entries++] = child_directory;
    return child_directory;
}

struct inode *get_inode(char *id, struct directory *directory)
{
    if (directory == NULL)
    {
        directory = &root_directory;
    }

    for (int i = 0; i < directory->total_entries; i++)
    {
        if (memcmp(directory->inode_entries[i]->meta_data->id, id, strlen(id)) == 0)
        {
            return directory->inode_entries[i];
        }
    }
    return 0x0;
}

struct directory *get_file_directory(char *id)
{
    size_t a = (size_t)&directory_table;
    for (int i = 0; i < directory_table.total_entries; i++)
    {
        if (memcmp(directory_table.directory_entries[i]->inode->meta_data->id, id, strlen(id)) == 0)
        {
            return directory_table.directory_entries[i];
        }
    }

    return 0x0;
}
