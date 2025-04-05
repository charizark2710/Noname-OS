#ifndef PARSER_H

#define PARSER_H

#include "string.h"
#include "status.h"
#include "memory.h"
#include "kheap.h"
#include "ext2.h"

#define MAX_PATH 4096
#define NAME_MAX 256
#define MAX_PATH_COMP 15

// 12 direct blocks, 3 indirect blocks
#define MAX_BLOCK 15

struct path_root
{
    struct inode *inodes;
    const char *path;
};

struct path_root *path_parser(const char *path);
void free_inode(struct inode *inode);

#endif