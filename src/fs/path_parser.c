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

void free_inode(struct inode *inode)
{
    kfree(inode);
}

// ex: /path/to/file
struct path_root *path_parser(const char *path, bool need_create)
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
    size_t root_directory = (size_t)get_file_directory("root");
    // ex: [path][to][file]
    for (const char *p = path; p != 0x0 && *p != '\0'; p++)
    {
        if (*p == '/')
        {
            // check if (*path_ptr)[c_index++] is directory
            if (index > 0)
            {
                struct directory *directory = get_file_directory(*path_ptr);
                if (directory == 0x0)
                {
                    if (!need_create)
                    {
                        status = PATH_INVALID;
                        break;
                    }
                    directory = create_dir(*path_ptr, 0, (struct directory *)root_directory);
                }
                root_directory = (size_t)directory;
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

    if (status == OK)
    {
        struct inode *first_inode = kmalloc(sizeof(struct inode));

        struct path_root *result = kmalloc(sizeof(struct path_root));
        result->full_path = path;
        result->parent_path = path_org_addr[index - 2];
        result->directory = (struct directory *)root_directory;
        result->inode = get_inode(*path_ptr, result->directory);
        kfree(path_org_addr);
        return result;
    }

    kfree(path_org_addr);

    return 0x0;
}

void *read_block(struct block *block)
{
    struct block *current = block;
    char *result = "";
    size_t start_index = (size_t)result;
    do
    {
        size_t size = current->end - current->start + 1;
        memcpy((void *)start_index, current->start, size);
        start_index += size;
        current = current->next;
    } while (current != 0x0);
    return result;
}

void *read_file(struct inode *inode)
{
    return read_block(inode->block);
}
