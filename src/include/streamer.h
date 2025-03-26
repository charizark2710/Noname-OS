#ifndef DISK_STREAMER_H
#define DISK_STREAMER_H

#include "disk.h"

struct disk_streamer
{
    int pos;
    struct disk *disk;
};
struct disk_streamer *new_streamer(int disk_id);
int seek_streamer(struct disk_streamer *streamer, int pos);
int read_streamer(struct disk_streamer *streamer, void *out, int len);
void close_streamer(struct disk_streamer *streamer);

#endif