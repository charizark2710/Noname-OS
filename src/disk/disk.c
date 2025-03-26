#include "../include/disk.h"

struct disk disk;

int disk_read_sector(int lba, int total_sectors, void *buf)
{
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total_sectors);
    outb(0x1F3, lba);
    outb(0x1F4, lba >> 8);
    outb(0x1F5, lba >> 16);
    outb(0x1F7, 0x20);

    unsigned short *ptr = (unsigned short *)buf;

    for (int b = 0; b < total_sectors; b++)
    {
        // Wait for the buffer to be ready
        char c = insb(0x1F7);
        while (!(c & 0x08))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int i = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
        }
    }
    return 0;
}

int read_from_disk(struct disk *d, int lba, int total_sectors, void *buf)
{
    if (d != &disk)
    {
        return -1;
    }
    return disk_read_sector(lba, total_sectors, buf);
}

void search_and_init()
{
    memset(&disk, 0, sizeof(disk));
    disk.type = 0;
    disk.size = SECTOR_SIZE;
}

struct disk *get_disk(int index)
{
    if (index != 0)
    {
        return 0;
    }
    return &disk;
}
