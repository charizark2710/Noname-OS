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

struct path_root
{
    const char *full_path;
    const char *parent_path;
    struct directory *directory;
    struct inode *inode;
};

struct path_root *path_parser(const char *path, bool need_create);
void free_inode(struct inode *inode);
void *read_file(struct inode *inode);

#endif