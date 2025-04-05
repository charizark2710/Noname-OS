#include "../include/path_parser.h"

int validate_path(const char *path)
{
    int len = strlen(path);
    if (len >= MAX_PATH)
    {
        return PATH_MAX_EXCEEDED;
    }

    // Path should be format /path/to/file
    if (len == 1 || memcmp(path, "/", 1) != 0)
    {
        return PATH_INVALID;
    }

    if (path[len - 1] == '/')
    {
        return PATH_INVALID;
    }

    return OK;
}

static struct path_root *get_or_create_root(const char *path)
{
    // TODO: implement create path root
    struct path_root *root = kmalloc(sizeof(struct path_root));
    root->path = path;
    return root;
}

struct inode *get_or_create_directory(struct inode *self_inode)
{
    // TODO: implement check if this inode is in directory_table
    // if it exist return the child_inode
    // else create and return the child_inode

    // create child_inode
    struct inode *child_inode = kmalloc(sizeof(struct inode));
    return child_inode;
}

// ex: [path][to][file]
struct inode *get_or_create_inode(char **path, struct inode *inode)
{
    // Check if next path exist
    if (*path != 0x0 || **path != '\0')
    {
        struct inode *next_inode = get_or_create_directory(inode);
        inode->block = 0x0;
        // TODO: implement get block
        get_or_create_inode(path + 1, next_inode);
    }
    else
    {
        inode->block = 0x0;
    }

    return inode;
}

void free_inode(struct inode *inode)
{
    kfree(inode);
}

int is_directory(char **name)
{
    // check if nama is exist in directory_table
    return 1;
}

// ex: /path/to/file
struct path_root *path_parser(const char *path)
{
    int status = validate_path(path);
    if (status != OK)
    {
        return 0;
    }

    char **path_ptr = kmalloc(sizeof(char *) * MAX_PATH_COMP);
    char **path_org_addr = (char **)(size_t)path_ptr;
    // *path_ptr = path_part;
    int index = 0;
    int c_index = 0;
    for (const char *p = path; p != 0x0 && *p != '\0'; p++)
    {
        if (*p == '/')
        {
            // check if (*path_ptr)[c_index++] is directory
            if (index > 0)
            {
                path_ptr++;
            }
            *path_ptr = kmalloc(sizeof(char) * NAME_MAX);
            index++;
            c_index = 0;
            continue;
        }
        (*path_ptr)[c_index++] = *p;
    }

    // Mark as end
    path_ptr[index][0] = '\0';

    struct inode *first_inode = kmalloc(sizeof(struct inode));

    struct inode *inodes = get_or_create_inode(path_org_addr, first_inode);
    struct path_root *result = get_or_create_root(path);
    result->inodes = inodes;

    // free everything
    // for (int i = 0; i < index; i++)
    // {
    //     kfree(path_org_addr[i]);
    // }
    kfree(path_org_addr);

    return 0x0;
}
