#ifndef DISK_H

#define DISK_H
#include "io.h"
#include "memory.h"
#include "../config.h"
struct disk
{
    int type;
    int size;
};

int read_from_disk(struct disk *d, int lba, int total_sectors, void *buf);
void search_and_init();
struct disk *get_disk(int index);

#endif