#include "../include/streamer.h"
#include "../include/memory.h"
#include "../include/kheap.h"

struct disk_streamer *new_streamer(int disk_id)
{
    struct disk *disk = get_disk(disk_id);
    if (!disk)
    {
        return 0x0;
    }

    struct disk_streamer *streamer = kmalloc(sizeof(struct disk_streamer));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
};

int seek_streamer(struct disk_streamer *streamer, int pos)
{
    streamer->pos = pos;
    return 0;
}

int read_streamer(struct disk_streamer *streamer, void *out, int len)
{
    int sector = streamer->pos / SECTOR_SIZE;
    int offset = streamer->pos % SECTOR_SIZE;
    char buf[SECTOR_SIZE];
    int res = read_from_disk(streamer->disk, sector, 1, buf);
    if (res < 0)
    {
        return res;
    }

    int total_to_read = len > SECTOR_SIZE ? SECTOR_SIZE : len;
    for (int i = 0; i < total_to_read; i++)
    {
        *(char *)out++ = buf[offset + i];
    }

    streamer->pos += total_to_read;
    if (len > SECTOR_SIZE)
    {
        res = read_streamer(streamer, out, len - SECTOR_SIZE);
    }

    return res;
}

void close_streamer(struct disk_streamer *streamer)
{
    kfree(streamer);
}
